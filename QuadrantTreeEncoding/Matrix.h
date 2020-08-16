#pragma once
#include "Library.h"

template<typename T>
class Matrix {

private:

    T* matrix;

    size_t rows, cols;

public:

    Matrix(size_t rows, size_t cols) {
        this->rows = rows;
        this->cols = cols;
        matrix = new T[rows * cols];
    }

    Matrix(size_t rows, size_t cols, T initialValue) {
        this(rows, cols);
        for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < cols; c++) {
                set(r, c, initialValue);
            }
        }
    }

    ~Matrix() {
        delete matrix;
    }
    
    T get(size_t row, size_t col) {
        assert(validPos(row, col));
        return matrix[row * cols + col];
    }

    void set(size_t row, size_t col, T value) {
        assert(validPos(row, col));
        matrix[row * cols + col] = value;
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
        return row >= 0 && row < rows&& col >= 0 && col < cols;
    }

};