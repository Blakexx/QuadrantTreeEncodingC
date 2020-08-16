#pragma once
#include "Library.h"
#include "StackFrame.h"
#include "MemoryController.h"
#include "Matrix.h"

using namespace std;

class QuadrantTreeEncoder{

private:

    template<typename T>
    static bool doPathSetup(StackFrame& frame, MemoryController* bits, Matrix<T>* matrix, T& defaultItem) {
        size_t prevLength = bits->size();
        if (frame.size() > 1) {
            bits->addBit(true);
        }
        bool gotData = encodeHelper(frame, bits, matrix, defaultItem);
        if (frame.size() > 1 && !gotData) {
            bits->erase(prevLength, bits->size());
            bits->addBit(false);
        }
        return gotData;
    }


    template<typename T>
    static bool encodeHelper(StackFrame& frame, MemoryController* bits, Matrix<T>* matrix, T& defaultItem) {
        int yPos = frame.yPos, xPos = frame.xPos;
        if (!matrix->validPos(yPos, xPos)) {
            return false;
        }
        if (frame.size() <= 1) {
            T item = matrix->get(yPos, xPos);
            if (item == defaultItem) {
                bits->addBit(false);
                return false;
            }
            bits->addBit(true);
            bits->addBits(&item);
            return true;
        }
        else {
            bool foundData = false;
            list<StackFrame>* children = frame.getChildren();
            for (auto it = children->begin(); it!=children->end(); it++) {
                foundData |= doPathSetup(*it, bits, matrix, defaultItem);
            }
            delete children;
            return foundData;
        }
    }
    
public:

    static int headerSize, dataSize, refSize;

    static void printAnalytics() {
        cout << "Header size: " << headerSize << endl;
        cout << "Data size: " << dataSize << endl;
        cout << "Ref size: " << refSize << endl;
    }

    template<typename T>
    static MemoryController* encodeMatrix(Matrix<T>* matrix) {
        MemoryController* bits = new MemoryController();
        int height = matrix->getRows();
        int width = matrix->getCols();
        assert(height > 0 && width > 0);
        refSize = 0;
        dataSize = 0;
        headerSize = 0;
        unordered_map<T, int> countMap;
        int maxCount = 0;
        T defaultItem;
        for (size_t r = 0; r < height; r++) {
            for (size_t c = 0; c < width; c++) {
                T item = matrix->get(r, c);
                if (++countMap[item] > maxCount) {
                    maxCount = countMap[item];
                    defaultItem = item;
                }
            }
        }
        assert(maxCount > 0);
        dataSize = (matrix->size() - maxCount) * sizeof(T);
        bits->addBits(&defaultItem);
        bits->addBits(&height);
        bits->addBits(&width);
        headerSize = bits->size();
        if (dataSize > 0) {
            StackFrame baseFrame(0, 0, height, width);
            doPathSetup(baseFrame, bits, matrix, defaultItem);
        }
        else {
            bits->addBit(false);
        }
        refSize = bits->size() - dataSize - headerSize;
        bits->trim();
        return bits;
    }

    template<typename T>
    static Matrix<T>* decodeMatrix(MemoryController* bits) {
        size_t index = 0;
        T defaultItem = *bits->getBits<T>(0);
        index += sizeof(T) * CHAR_BIT;
        int height = *bits->getBits<int>(index);
        index += sizeof(height) * CHAR_BIT;
        int width = *bits->getBits<int>(index);
        index += sizeof(width) * CHAR_BIT;
        list<StackFrame> stack;
        stack.push_back(StackFrame(0, 0, height, width));
        Matrix<T>* matrix = new Matrix<T>(height,width);
        while (stack.size() > 0 && index<bits->size()) {
            bool nextInst = bits->getBit(index++);
            StackFrame current = stack.back();
            bool readMode = current.width <= 1 && current.height <= 1;;
            if (nextInst) {
                if (readMode) {
                    T data = *bits->getBits<T>(index);
                    index += sizeof(T) * CHAR_BIT;
                    matrix->set(current.yPos,current.xPos,data);
                    stack.pop_back();
                }
                else {
                    StackFrame::pushFrame(stack);
                }
            }
            else {
                for (int r = 0; r<current.height; r++) {
                    for (int c = 0; c < current.width; c++) {
                        matrix->set(current.yPos+r, current.xPos+c, defaultItem);
                    }
                }
                stack.pop_back();
            }
        }
        return matrix;
    }

};