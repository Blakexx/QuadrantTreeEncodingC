#include "Library.h"
#include "Tests.h"
#include "MemoryController.h"
#include "QuadrantTreeEncoder.h"
#include "BitList.h"

int main()
{
    auto generator = [](Matrix<int>* matrix, size_t count) {
        for (size_t num = 0; num < count; num++) {
            int toSet = (rand() % 127) + 1;
            matrix->set(num, toSet);
        }
        matrix->randomShuffle();
    };
    auto intTester = [](int data)->bool {
        return data>=0;
    };
    auto doubleTester = [](double data)->bool {
        return data >= 0 && data <= 1;
    };
    const size_t r = readData<int>("Rows", intTester);
    const size_t c = readData<int>("Cols", intTester);
    const double fullness = readData<double>("Fullness", doubleTester);
    Matrix<int>* matrix = generateMatrix<int>(r,c,fullness,generator,0);
    //printMatrix(matrix);
    cout << "Begin Encoding..." << endl;
    MemoryController* bits = QuadrantTreeEncoder::encodeMatrix(matrix);
    QuadrantTreeEncoder::printAnalytics();
    //cout << bits->toStringAfter(QuadrantTreeEncoder::headerSize) << endl;
    Matrix<int>* decoded = QuadrantTreeEncoder::decodeMatrix<int>(bits);
    cout << "Decoded: " << endl;
    //printMatrix(decoded);
    delete bits;
    delete matrix;
    delete decoded;
    while (true);
}
