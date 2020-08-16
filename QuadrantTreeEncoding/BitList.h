#pragma once
#include "Library.h"

class BitList {

private:

    byte* container;

    size_t bitLength;

    byte generateMask(int index, int size) {
        assert(size > 0 && size <= 8);
        byte mask{ 255 };
        mask <<= CHAR_BIT - size;
        mask >>= index;
        return mask;
    }

    string rayToString(byte* list, size_t length) {
        string ret = "[";
        for (size_t i = 0; i < length; i++) {
            ret.append(to_string(to_integer<int>(list[i])));
            if (i != length - 1) {
                ret.append(", ");
            }
        }
        ret.append("]");
        return ret;
    }


public:

    BitList(size_t bitLength) {
        assert(bitLength > 0);
        this->bitLength = bitLength;
        size_t byteLength = roundUpDiv(bitLength, CHAR_BIT);
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

    void setRaw(size_t index, byte* bitData, size_t N) {
        if (N > 1) {
            //cout << "Current State: " << rayToString(container, roundUpDiv(length(), CHAR_BIT)) << endl;
            //cout << "Setting: (" << index << ", " << N << ") = " << rayToString(bitData, roundUpDiv(N, CHAR_BIT)) << endl;
        }
        assert(index >= 0 && index + N <= length());
        if (N == 0) {
            return;
        }
        size_t freeBits = 0;
        size_t bitIndex = index % CHAR_BIT;
        size_t conIndex = index / CHAR_BIT;
        for (size_t i = 0; i < N; i += freeBits) {
            if (bitIndex == CHAR_BIT) {
                conIndex++;
                bitIndex = 0;
            }
            size_t dataConIndex = i / CHAR_BIT;
            size_t dataBitIndex = i % CHAR_BIT;
            freeBits = min(N - i, min(CHAR_BIT - dataBitIndex, CHAR_BIT - bitIndex));
            byte dataByte = bitData[dataConIndex];
            dataByte <<= dataBitIndex;
            dataByte >>= bitIndex;
            byte mask = generateMask(bitIndex, freeBits);
            container[conIndex] &= ~mask;
            container[conIndex] |= dataByte & mask;
            bitIndex += freeBits;
        }
    }

    template<typename T>
    void set(size_t index, T* value) {
        setRaw(index,(byte*)value,sizeof(T)*CHAR_BIT);
    }

    void set(size_t index, bool value) {
        signed char charVal = value ? 255 : 0;
        setRaw(index,(byte*)&charVal,1);
    }

    byte* getRaw(size_t index, size_t N) {
        if (N > 1) {
            //cout << "Current State: " << rayToString(container, roundUpDiv(length(), CHAR_BIT)) << endl;
            //cout << "Getting: " << index << ", " << N << endl;
        }
        assert(index >= 0 && index + N <= length());
        size_t byteLength = roundUpDiv(N, CHAR_BIT);
        byte* returned = new byte[byteLength];
        if (N == 0) {
            return returned;
        }
        for (size_t i = 0; i < byteLength; i++) {
            returned[i] = byte{ 0 };
        }
        if (N > 1) {
            //cout << "Initial: " << rayToString(returned, byteLength) << endl;
        }
        size_t freeBits = 0;
        size_t bitIndex = index % CHAR_BIT;
        size_t conIndex = index / CHAR_BIT;
        for (size_t i = 0; i < N; i += freeBits) {
            if (bitIndex == CHAR_BIT) {
                conIndex++;
                bitIndex = 0;
            }
            size_t dataConIndex = i / CHAR_BIT;
            size_t dataBitIndex = i % CHAR_BIT;
            freeBits = min(N - i, min(CHAR_BIT - dataBitIndex, CHAR_BIT - bitIndex));
            byte data = container[conIndex] & generateMask(bitIndex, freeBits);
            data <<= bitIndex;
            data >>= dataBitIndex;
            returned[dataConIndex] |= data;
            bitIndex += freeBits;
        }
        if (N > 1) {
            //cout << "Returned: " << rayToString(returned, byteLength) << endl;
        }
        return returned;
    }

    template<typename T>
    T* get(size_t index) {
        return (T*)getRaw(index, sizeof(T) * CHAR_BIT);
    }

    bool get(size_t index) {
        return *(bool*)getRaw(index,1) != 0;
    }

    void erase(size_t start, size_t end) {
        assert(start < end && start >= 0 && end <= length());
    }

    string chunkString(size_t index, size_t length) {
        string ret = "";
        for (size_t i = index; i < index+length; i++) {
            ret.append(get(i) ? "1" : "0");
        }
        return ret;
    }

    string toString() {
        return chunkString(0, length());
    }

};