#pragma once
#include <cmath>
#include <functional>
#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map> 
#include <cstddef>
#include <limits.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <utility>

using namespace std;

inline double logBase(size_t value, size_t base) {
    return log10(value) / log10(base);
}

inline size_t logBaseCeil(size_t value, size_t base) {
    return (size_t)ceil(logBase(value,base));
}

inline size_t roundUpDiv(size_t numerator, size_t denominator) {
    return (size_t)ceil((double)numerator / denominator);
}