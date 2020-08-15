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

using namespace std;

inline size_t logBaseCeil(size_t value, size_t base) {
    return (size_t)ceil(log10(value) / log10(base));
}