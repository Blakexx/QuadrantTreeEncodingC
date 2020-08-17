#pragma once
#include "Library.h"

template<typename T>
class Matrix {

private:

    T* matrix;

    size_t rows, cols;

    bool validIndex(size_t index) {
        return index >= 0 && index < rows * cols;
    }

public:

    Matrix(size_t rows, size_t cols) {
        this->rows = rows;
        this->cols = cols;
        matrix = new T[rows * cols];
    }

    Matrix(size_t rows, size_t cols, T initialValue) : Matrix(rows,cols) {
        for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < cols; c++) {
                set(r, c, initialValue);
            }
        }
    }

    ~Matrix() {
        delete[] matrix;
    }

    T get(size_t rawIndex) {
        assert(validIndex(rawIndex));
        return matrix[rawIndex];
    }
    
    T get(size_t row, size_t col) {
        return get(row * cols + col);
    }

    void set(size_t rawIndex, T value) {
        assert(validIndex(rawIndex));
        matrix[rawIndex] = value;
    }

    void set(size_t row, size_t col, T value) {
        set(row * cols + col, value);
    }

    size_t getRows() {
        return rows;
    }

    size_t getCols() {
        return cols;
    }

    size_t size() {
        return rows * cols;
    }

    bool validPos(size_t row, size_t col) {
        return validIndex(row * cols + col);
    }

    void randomShuffle() {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(&matrix[0],&matrix[rows*cols-1], default_random_engine(seed));
    }

};