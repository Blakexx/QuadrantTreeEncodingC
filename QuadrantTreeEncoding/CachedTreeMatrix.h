#pragma once
#include "Library.h"
#include "Functions.h"
#include "Matrix.h"

template<typename T, size_t N>
class CachedTreeMatrix {

    CachedTreeMatrix(Matrix<T>* matrix, BitEncoder<T, N> encoder, BitDecoder<T, N> decoder);
    
};