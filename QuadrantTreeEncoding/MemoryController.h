#pragma once
#include "Library.h"
#include "Functions.h"
#include "BitList.h"

using namespace std;

class MemoryController {

private:

    BitList bits;
    size_t bitSize;

    void ensureCapacity(size_t toAdd) {

    }

public:

    MemoryController() : bits(10) {
        bitSize = 0;
    }

    bool getBit(size_t index) {
        return bits.get(index);
    }

    template<class T>
    T* getBits(size_t index) {
        return bits.get<T>(index);
    }

    void addBit(bool b) {
        
    }

    template<class T>
    void addBits(T value) {
        
    }

    void setBit(size_t index, bool bit) {
        bits.set(index, bit);
    }

    template<class T>
    void setBits(size_t index, T value) {
        bits.set<T>(index, value);
    }

    void insert(size_t index, bool value) {

    }

    template<class T>
    void insert(size_t index, T value) {

    }

    void erase(size_t start, size_t end) {
        
    }

    void trim() {
        BitList trimmed(size());

        
    }

    size_t size() {
        return bitSize;
    }

    string toStringAfter(size_t index) {
        string ret = "";
        for (size_t i = index; i < size(); i++) {
            ret.append(getBit(i) ? "1" : "0");
        }
        return ret;
    }

    string toString() {
        return toStringAfter(0);
    }

};