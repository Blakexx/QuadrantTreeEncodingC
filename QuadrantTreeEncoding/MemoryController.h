#pragma once
#include "Library.h"
#include "Functions.h"
#include "BitList.h"

using namespace std;

class MemoryController : public vector<bool> {

public:

    vector<bool>::reference getBit(size_t index) {
        return at(index);
    }

    template<size_t N>
    bitset<N> getBits(size_t index) {
        bitset<N> bits;
        for (int i = 0; i < N; i++) {
            bits[i] = at(index + i);
        }
        return bits;
    }

    template<class T, size_t N>
    T getBits(size_t index, BitDecoder<T, N> decoder) {
        return decoder(getBits<N>(index));
    }

    void addBit(bool b) {
        push_back(b);
    }

    template<size_t N>
    void addBits(bitset<N> bits) {
        for (int i = 0; i < N; i++) {
            push_back(bits[i]);
        }
    }

    template<class T, size_t N>
    void addBits(T value, BitEncoder<T, N> encoder) {
        addBits(encoder(value));
    }

    void setBit(size_t index, bool bit) {
        at(index) = bit;
    }

    template<size_t N>
    void setBits(size_t index, bitset<N> bits) {
        for (int i = 0; i < N; i++) {
            at(index + i) = bits[i];
        }
    }

    template<class T, size_t N>
    void setBits(size_t index, T value, BitEncoder<T, N> encoder) {
        setBits(index, encoder(value));
    }

    template<size_t N>
    void insert(size_t index, bitset<N> bits) {
        vector<bool>::insert(begin() + index, N, 0);
        setBits(index, bits);
    }

    template<class T, size_t N>
    void insert(size_t index, T value, BitEncoder<T, N> encoder) {
        insert(index, encoder(value));
    }

    void erase(size_t start, size_t end) {
        vector<bool>::erase(begin() + start, begin() + end);
    }

    void trim() {
        shrink_to_fit();
    }

    string toStringAfter(size_t index) {
        string ret = "";
        for (size_t i = index; i < size(); i++) {
            ret.append(at(i) ? "1" : "0");
        }
        return ret;
    }

    string toString() {
        return toStringAfter(0);
    }

};