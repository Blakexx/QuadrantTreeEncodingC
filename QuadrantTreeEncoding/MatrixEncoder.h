#pragma once
#include "Library.h"
#include "MemoryController.h"
#include "Matrix.h"
#include "Functions.h"

using namespace std;

class MatrixEncoder {

public:

    static void printAnalytics();

    template<typename T, size_t N>
    static MemoryController encodeMatrix(Matrix<T>* matrix, BitEncoder<T, N> dataEncoder);

    template<typename T, size_t N>
    static Matrix<T> decodeMatrix(MemoryController* bits, BitDecoder<T, N> dataDecoder);

};