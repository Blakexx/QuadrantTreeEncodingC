#pragma once
#include "Library.h"

class BitList {

private:

    constexpr int bitsPerByte() {
        return 8;
    }

    byte* container;

    size_t bitLength;

    byte generateMask(int index, int size) {
        assert(size > 0 && size <= 8);
        byte mask{ 255 };
        mask <<= bitsPerByte() - size;
        mask >>= index;
        return mask;
    }

    template<typename T, size_t N>
    void set(size_t index, T value) {
        assert(index >= 0 && index+N <= bitLength);
        byte* bitData = (byte*)&value;
        size_t freeBits = 0;
        size_t bitIndex = index % bitsPerByte();
        size_t conIndex = index / bitsPerByte();
        for (size_t i = 0; i < N; i += freeBits) {
            if (bitIndex == bitsPerByte()) {
                conIndex++;
                bitIndex = 0;
            }
            size_t dataConIndex = i / bitsPerByte();
            size_t dataBitIndex = i % bitsPerByte();
            freeBits = min(N - i, min(bitsPerByte() - dataBitIndex, bitsPerByte() - bitIndex));
            byte dataByte = bitData[dataConIndex];
            dataByte <<= dataBitIndex;
            dataByte >>= bitIndex;
            byte mask = generateMask(bitIndex, freeBits);
            container[conIndex] &= ~mask;
            container[conIndex] |= dataByte & mask;
            bitIndex += freeBits;
        }
    }

    template<typename T, size_t N>
    T* get(size_t index) {
        assert(index >= 0 && index+N <= bitLength);
        size_t byteLength = (size_t)ceil(N / (double)bitsPerByte());
        byte* returned = new byte[byteLength];
        for (size_t i = 0; i < byteLength; i++) {
            returned[i] = byte{ 0 };
        }
        size_t freeBits = 0;
        size_t bitIndex = index % bitsPerByte();
        size_t conIndex = index / bitsPerByte();
        for (size_t i = 0; i < N; i += freeBits) {
            if (bitIndex == bitsPerByte()) {
                conIndex++;
                bitIndex = 0;
            }
            size_t dataConIndex = i / bitsPerByte();
            size_t dataBitIndex = i % bitsPerByte();
            freeBits = min(N - i, min(bitsPerByte() - dataBitIndex, bitsPerByte() - bitIndex));
            byte data = container[conIndex] & generateMask(bitIndex, freeBits);
            data <<= bitIndex;
            data >>= dataBitIndex;
            returned[dataConIndex] |= data;
            bitIndex += freeBits;
        }
        return (T*)returned;
    }


public:

    BitList(size_t bitLength) {
        assert(bitLength > 0);
        this->bitLength = bitLength;
        size_t byteLength = (size_t)ceil(bitLength / (double)bitsPerByte());
        container = new byte[byteLength];
        for (size_t i = 0; i < byteLength; i++) {
            container[i] = byte{ 0 };
        }
    }

    ~BitList() {
        delete container;
    }

    size_t length() {
        return bitLength;
    }

    template<typename T>
    void set(size_t index, T value) {
        set<T,sizeof(T)*8>(index,value);
    }

    void set(size_t index, bool value) {
        set<signed char, 1>(index,value?255:0);
    }

    template<typename T>
    T* get(size_t index) {
        return get<T,sizeof(T)*8>(index);
    }

    bool get(size_t index) {
        return *get<bool, 1>(index);
    }

    void erase(size_t start, size_t end) {
        assert(start < end&& start >= 0 && end <= length());
    }

    template<typename T>
    void insert(size_t index, T value) {
        assert(index >= 0 && index <= length());

    }

    void insert(size_t index, bool value) {
        assert(index >= 0 && index <= length());

    }

    void changeSize(size_t newSize) {
        assert(newSize >= 0);
    }

    string toStringAfter(size_t index) {
        string ret = "";
        for (size_t i = index; i < bitLength; i++) {
            ret.append(get(i) ? "1" : "0");
        }
        return ret;
    }

    string toString() {
        return toStringAfter(0);
    }

};