#pragma once
#include <functional>
#include <bitset>

using namespace std;

template<typename T, size_t N>
using BitEncoder = function<bitset<N>(T)>;

template<typename T, size_t N>
using BitDecoder = function<T(bitset<N>)>;

template<typename F, typename S, typename R>
using BiFunction = function<R(F, S)>;

template<typename T, size_t N = sizeof(T) * 8>
const BitEncoder<T, N> intEncoder = [](T value)->bitset<N> {
    static_assert(is_integral<T>::value, "Integral required.");
    return bitset<N>(value);
};

template<typename T, size_t N = sizeof(T) * 8>
const BitDecoder<T, N> intDecoder = [](bitset<N> bits)->T {
    static_assert(is_integral<T>::value, "Integral required.");
    return (T)bits.to_ulong();
};