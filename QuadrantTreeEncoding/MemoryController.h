#pragma once
#include "Library.h"
#include "Functions.h"
#include "BitList.h"

using namespace std;

class MemoryController {

private:

    BitList* bits;
    size_t bitSize;

    void ensureCapacity(size_t toAdd) {
        //cout << "Ensure Capacity: " << toAdd << ", " << size() << endl;
        if (toAdd <= 0) {
            cout << "toAdd <= 0 " << endl;
            return;
        }
        if (size() + toAdd <= bits->length()) {
            //cout << "ENOUGH CAPACITY" << endl;
            bitSize += toAdd;
            return;
        }
        int toMult = logBaseCeil(size() + toAdd,2) - logBaseCeil(bits->length(), 2);
        toMult <<= 1;
        BitList* newList = new BitList(bits->length() * toMult);
        //cout << "COPYING TO NEW CONTAINER: " << bits->toString() << endl;
        newList->setRaw(0,bits->getRaw(0,size()),size());
        delete bits;
        bits = newList;
        //cout << "newList: " << newList->toString() << endl;
        //cout << "COPY FINISHED: " << bits->toString() << endl;
        bitSize += toAdd;
    }

    void copyBits(size_t origin, size_t dest, size_t toCopy) {
        assert(validIndex(origin)&&validIndex(origin+toCopy));
        //cout << "COPYING: " << origin << ", " << dest << ", " << toCopy << ", " << size() << endl;
        //cout << dest + toCopy - size() << endl;
        if (dest + toCopy > size()) {
            ensureCapacity(dest + toCopy - size());
        }
        //cout << "Copy Begin: " << origin << ", " << toCopy << ", " << bits->toString() << endl;
        if (toCopy > 0) {
            bits->setRaw(dest, bits->getRaw(origin, toCopy), toCopy);
        }
        //cout << "Copy End: " << bits->toString() << endl;
    }

    bool validIndex(size_t index) {
        return index >= 0 && index <= size();
    }

public:

    MemoryController() {
        bits = new BitList(8);
        bitSize = 0;
    }

    ~MemoryController(){
        delete bits;
    }

    bool getBit(size_t index) {
        return bits->get(index);
    }

    template<class T>
    T* getBits(size_t index) {
        return bits->get<T>(index);
    }

    void addBit(bool b) {
        insert(size(), b);
    }

    template<class T>
    void addBits(T* value) {
        insert<T>(size(), value);
    }

    void setBit(size_t index, bool bit) {
        bits->set(index, bit);
    }

    template<typename T>
    void setBits(size_t index, T* value) {
        bits->set<T>(index, value);
    }

    void insert(size_t index, bool value) {
        assert(validIndex(index));
        copyBits(index, index + 1, size()-index);
        bits->set(index, value);
    }

    template<typename T>
    void insert(size_t index, T* value) {
        assert(validIndex(index));
        copyBits(index, index + sizeof(T) * CHAR_BIT, size()-index);
        bits->set<T>(index, value);
    }

    void erase(size_t start, size_t end) {
        assert(start >= 0 && end <= size() && start <= end);
        //cout << "ERASE: [" << start << ", " << end << ")" << endl;
        copyBits(end,start,size()-end);
        bitSize -= (end - start);
    }

    void trim() {
        BitList* trimmed = new BitList(size());
        trimmed->setRaw(0,bits->getRaw(0,size()),size());
        delete bits;
        bits = trimmed;
    }

    size_t size() {
        return bitSize;
    }

    string chunkString(size_t index, size_t length) {
        assert(validIndex(index) && validIndex(index + length));
        return bits->chunkString(index, length);
    }

    string toStringAfter(size_t index) {
        assert(validIndex(index));
        return chunkString(index,size()-index);
    }

    string toString() {
        return toStringAfter(0);
    }

};