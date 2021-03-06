#pragma once
#include "Library.h"
#include "Matrix.h"

template<typename T>
Matrix<T>* generateMatrix(size_t rows, size_t cols, double fullness, const function<void(Matrix<T>*, size_t)>& generator, T defaultValue) {
    size_t count = (size_t)round(rows * cols * fullness);
    vector<size_t> points;
    Matrix<T>* matrix = new Matrix<T>(rows,cols,defaultValue);
    generator(matrix, count);
    return matrix;
}

template<typename T>
T readData(string name, function<bool(T)> tester) {
    T value;
    bool failed = false;
    do {
        cin.clear();
        if (failed) {
            cin.ignore(1000, '\n');
        }
        cout << endl << name << ": ";
        cin >> value;
        failed = cin.fail();
    } while (failed || !tester(value));
    return value;
}

//void readWriteTester(double fullness, double cachePercent);

//void printIf(string toPrint, bool doPrint);

//double* runSetTests(bool doPrint);