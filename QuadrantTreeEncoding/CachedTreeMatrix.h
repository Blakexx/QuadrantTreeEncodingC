#pragma once
#include "Library.h"
#include "Functions.h"
#include "Matrix.h"
#include "MemoryController.h"
#include "LRUCache.h"
#include "QuadrantTreeEncoder.h"

template<typename T, size_t N>
class CachedTreeMatrix {

private:

    MemoryController* bits;
    LRUCache<size_t,size_t>* cache;
    double cachePercent;
    list<pair<StackFrame, size_t>> cacheQueue;

    size_t frameHash(StackFrame frame) {
        return frame.yPos * width() + frame.xPos;
    }

public:

    CachedTreeMatrix(Matrix<T>* matrix, double cachePercent=.01) {
        bits = QuadrantTreeEncoder::encodeMatrix(matrix);
        StackFrame baseFrame(0, 0, matrix->getRows(), matrix->getCols());
        cache = new LRUCache<size_t,size_t>((size_t)round(baseFrame.size()*cachePercent));
        
    }

    ~CachedTreeMatrix() {
        delete bits;
        delete cache;
    }

    T defaultItem() {
        return bits->getBits<T>(0);
    }

    size_t height() {
        return bits->getBits<size_t>(sizeof(T));
    }

    size_t width() {
        return bits->getBits<size_t>(sizeof(T)+sizeof(size_t));
    }

    void trim() {
        bits->trim();
    }

    void putIntoCache(StackFrame frame, size_t index) {

    }

    size_t getFromCache(StackFrame frame, size_t parentIndex, bool doCache) {

    }
    
};