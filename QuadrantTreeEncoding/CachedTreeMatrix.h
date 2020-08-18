#pragma once
#include "Library.h"
#include "Functions.h"
#include "Matrix.h"
#include "MemoryController.h"
#include "LRUCache.h"
#include "QuadrantTreeEncoder.h"

template<typename T>
class CachedTreeMatrix {

private:

    using FrameCollection = pair<size_t, list<StackFrame>>;
    using FrameInfo = pair<StackFrame, size_t>;

    MemoryController* bits;
    LRUCache<size_t,size_t>* cache;
    double cachePercent;
    list<FrameInfo> cacheQueue;

    size_t frameHash(StackFrame& frame) {
        return frame.yPos * width() + frame.xPos;
    }

    size_t headerSize() {
        return CHAR_BIT*(sizeof(T) + 2 * sizeof(size_t));
    }

    void emptyCacheQueue() {
        while (cacheQueue.size() > 0) {
            FrameInfo toCache = cacheQueue.back();
            if (toCache.second == -1) {
                cache->improveItem(frameHash(toCache.first));
            }
            else {
                putIntoCache(toCache.first, toCache.second);
            }
            cacheQueue.pop_back();
        }
    }

    void putIntoCache(StackFrame& frame, size_t index) {
        if (frame.parent != NULL && frame.quadrant == 0) {
            return;
        }
        if (index != -1) {
            cache->put(frameHash(frame), index);
        }
        else {
            cache->remove(frameHash(frame));
        }
    }

    size_t getFromCache(StackFrame& frame, size_t parentIndex, bool doCache) {
        size_t hash = frameHash(frame);
        if (doCache) {
            cache->improveItem(hash);
        }
        if (frame.parent == NULL) {
            return 0;
        }
        else if (frame.quadrant == 0) {
            return bits->getBit(parentIndex) ? 1 : -1;
        }
        if (cache->contains(hash)) {
            return cache->getNoCache(hash);
        }
        return -1;
    }

    size_t getFromCache(StackFrame& frame, size_t parentIndex) {
        return getFromCache(frame, parentIndex, false);
    }

    FrameCollection getClosestIndexFromCache(size_t r, size_t c) {
        StackFrame baseFrame(0, 0, height(), width());
        cacheQueue.push_back(FrameInfo(baseFrame,0));
        size_t dataIndex = headerSize();
        while (true) {
            StackFrame current = baseFrame.getChildContaining(r, c);
            size_t cacheIndex = getFromCache(current,dataIndex);
            if (cacheIndex != -1) {
                dataIndex += cacheIndex;
                if (current.size() == 1) {
                    list<StackFrame> toReturn;
                    toReturn.push_back(current);
                    return FrameCollection(dataIndex,toReturn);
                }
                else {
                    cacheQueue.push_back(FrameInfo(current,cacheIndex));
                    baseFrame = current;
                }
            }
            else {
                list<StackFrame> toReturn;
                toReturn.push_back(current);
                list<StackFrame>* frames = baseFrame.getChildrenBefore(r,c);
                for (list<StackFrame>::reverse_iterator rit = frames->rbegin(); rit != frames->rend(); rit++) {
                    StackFrame frame = *rit;
                    toReturn.push_back(frame);
                    cacheIndex = getFromCache(frame,dataIndex);
                    if (cacheIndex != -1) {
                        cacheQueue.push_back(FrameInfo(frame, cacheIndex));
                        dataIndex += cacheIndex;
                        break;
                    }
                }
                delete frames;
                if (cacheIndex == -1) {
                    toReturn.clear();
                    toReturn.push_back(baseFrame);
                    cacheQueue.pop_back();
                }
                return FrameCollection(dataIndex,toReturn);
            }
        }
    }

    FrameInfo decodeUntil(size_t r, size_t c, FrameCollection& collection) {
        size_t dataIndex = collection.first;
        list<StackFrame> frames = collection.second;
        StackFrame currentFrame = frames.back();
        StackFrame goal(r,c,1,1);
        size_t parentIndex = dataIndex - getFromCache(currentFrame,dataIndex-1);
        while (dataIndex < bits->size() && goal != currentFrame) {
            if (currentFrame.contains(r, c)) {
                cacheQueue.push_back(FrameInfo(currentFrame,dataIndex-parentIndex));
                parentIndex = dataIndex;
            }
            if (bits->getBit(dataIndex)) {
                if (currentFrame.size() <= 1) {
                    frames.pop_back();
                    dataIndex += sizeof(T) * CHAR_BIT;
                }
                else {
                    StackFrame::pushFrame(frames);
                }
            }
            else {
                if (currentFrame.contains(goal.yPos, goal.xPos)) {
                    return FrameInfo(currentFrame, dataIndex);
                }
                frames.pop_back();
            }
            dataIndex++;
            currentFrame = frames.back();
        }
        cacheQueue.push_back(FrameInfo(currentFrame, dataIndex - parentIndex));
        return FrameInfo(currentFrame,dataIndex);
    }

    MemoryController* encodeChunk(StackFrame& currentFrame, size_t r, size_t c, T& data, size_t dataIndex) {
        MemoryController* chunk = new MemoryController();
        list<StackFrame> frames;
        frames.push_back(currentFrame);
        size_t parentIndex = dataIndex - getFromCache(currentFrame, dataIndex - 1);
        while (frames.size() > 0) {
            currentFrame = frames.back();
            if (currentFrame.contains(r, c)) {
                cacheQueue.push_back(FrameInfo(currentFrame, dataIndex - parentIndex));
                parentIndex = dataIndex;
                chunk->addBit(true);
                if (currentFrame.size() == 1) {
                    chunk->addBits<T>(data);
                    frames.pop_back();
                    dataIndex += sizeof(T)*CHAR_BIT;
                }
                else {
                    StackFrame::pushFrame(frames);
                }
            }
            else {
                chunk->addBit(false);
                frames.pop_back();
            }
            dataIndex++;
        }
        return chunk;
    }

    bool hasData(StackFrame frame, size_t ignore, size_t size, size_t index) {
        if (!bits->getBit(index)) {
            return false;
        }
        size_t iterations = frame.width == 1 || frame.height == 1 ? 2 : 4;
        for (size_t i = 1; i <= iterations; i++) {
            if (i < ignore) {
                if (bits->getBit(index + i)) {
                    return true;
                }
            }
            else if (i > ignore) {
                if (bits->getBit(index + size + i)) {
                    return true;
                }
            }
        }
        return false;
    }

public:

    CachedTreeMatrix(Matrix<T>* matrix, double cachePercent=.01) {
        bits = QuadrantTreeEncoder::encodeMatrix(matrix);
        StackFrame baseFrame(0, 0, matrix->getRows(), matrix->getCols());
        cache = new LRUCache<size_t,size_t>((size_t)round(baseFrame.size()*cachePercent));
        putIntoCache(baseFrame, 0);
        trim();
    }

    ~CachedTreeMatrix() {
        delete bits;
        delete cache;
    }

    T get(size_t r, size_t c) {
        assert(r >= 0 && r < height() && c >= 0 && c < width());
        FrameCollection collection = getClosestIndexFromCache(r, c);
        size_t dataIndex = decodeUntil(r, c, collection).second;
        emptyCacheQueue();
        if (dataIndex < bits->size() && bits->getBit(dataIndex)) {
            return bits->getBits<T>(dataIndex + 1);
        }
        return defaultItem();
    }

    T set(size_t r, size_t c, T& data) {
        assert(r >= 0 && r < height() && c >= 0 && c < width());
        FrameCollection collection = getClosestIndexFromCache(r, c);
        FrameInfo frameInfo = decodeUntil(r, c, collection);
        StackFrame& baseFrame = frameInfo.first;
        size_t dataIndex = frameInfo.second;
        if (defaultItem() != data) {
            if (bits->getBit(dataIndex)) {
                bits->setBits<T>(dataIndex + 1, data);
            }
            else {
                MemoryController* toAdd = encodeChunk(frameInfo.first, r, c, data, dataIndex);
                bits->copyBits(dataIndex+1,dataIndex+toAdd->size(),bits->size()-dataIndex-1);
                bits->setContollerBits(dataIndex,toAdd);
                for (list<FrameInfo>::reverse_iterator i = cacheQueue.rbegin(); i != cacheQueue.rend(); i++) {
                    StackFrame current = (*i).first;
                    if (current.size() == 1 || !current.contains(r, c) || !cache->contains(frameHash(current))) {
                        continue;
                    }
                    list<StackFrame>* frames = current.getChildrenAfter(r, c);
                    for (auto it = frames->begin(); it != frames->end(); it++) {
                        StackFrame frame = *it;
                        size_t index = getFromCache(frame, 0);
                        if (index != -1) {
                            putIntoCache(frame, index + toAdd->size() - 1);
                        }
                    }
                    delete frames;
                }
                delete toAdd;
            }
        }
        else {
            if (bits->getBit(dataIndex)) {
                size_t removed = sizeof(T) * CHAR_BIT;
                size_t deleteStart = dataIndex + 1, deleteEnd = dataIndex + removed + 1;
                size_t ignoreIndex = -1;
                for (list<FrameInfo>::reverse_iterator it = cacheQueue.rbegin(); it != cacheQueue.rend(); it++) {
                    FrameInfo info = *it;
                    StackFrame frame = info.first;
                    size_t offset = info.second;
                    bool contains = frame.contains(r, c);
                    if (frame.size() == 1 || !contains) {
                        if (contains) {
                            dataIndex -= offset;
                            ignoreIndex = offset;
                        }
                        continue;
                    }
                    if (!hasData(frame, ignoreIndex, removed, dataIndex)) {
                        size_t children = frame.width == 1 || frame.height == 1 ? 2 : 4;
                        deleteStart = min(dataIndex + 1, deleteStart);
                        deleteEnd = max(dataIndex + 1 + removed + children, deleteEnd);
                        while (cacheQueue.back().first != frame) {
                            cacheQueue.pop_back();
                        }
                        it = cacheQueue.rbegin();
                        list<StackFrame>* frames = frame.getChildren();
                        for (auto it = frames->begin(); it != frames->end(); it++) {
                            putIntoCache(*it, -1);
                        }
                        delete frames;
                        removed += children;
                        dataIndex -= offset;
                        ignoreIndex = offset;
                    }   
                    else {
                        break;
                    }
                }
                bits->erase(deleteStart, deleteEnd);
                bits->setBit(deleteStart - 1, false);
                for (list<FrameInfo>::reverse_iterator i = cacheQueue.rbegin(); i != cacheQueue.rend(); i++) {
                    StackFrame current = (*i).first;
                    if (current.size() == 1 || !current.contains(r, c) || !cache->contains(frameHash(current))) {
                        continue;
                    }
                    list<StackFrame>* frames = current.getChildrenAfter(r, c);
                    for (auto it = frames->begin(); it != frames->end(); it++) {
                        StackFrame frame = *it;
                        size_t index = getFromCache(frame, 0);
                        if (index != -1) {
                            putIntoCache(frame, index - removed);
                        }
                    }
                    delete frames;
                }
            }
        }
        emptyCacheQueue();
        cout << "AFTER ERASE: " << bits->toStringAfter(headerSize()) << endl;
        return data;
    }

    T defaultItem() {
        return bits->getBits<T>(0);
    }

    size_t height() {
        return bits->getBits<size_t>(sizeof(T)*CHAR_BIT);
    }

    size_t width() {
        return bits->getBits<size_t>(CHAR_BIT*(sizeof(T)+sizeof(size_t)));
    }

    void trim() {
        bits->trim();
    }

    Matrix<T>* toMatrix() {
        return QuadrantTreeEncoder::decodeMatrix<T>(bits);
    }

    string toString() {
        Matrix<T>* decoded = toMatrix();
        string returned = decoded->toString();
        delete decoded;
        return returned;
    }
    
};