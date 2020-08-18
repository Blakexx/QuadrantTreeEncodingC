#include "Library.h"
#include "Tests.h"
#include "MemoryController.h"
#include "QuadrantTreeEncoder.h"
#include "BitList.h"
#include "LRUCache.h"
#include "CachedTreeMatrix.h"

int main()
{
    srand((unsigned int)time(NULL));
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
    cout << matrix->toString() << endl;
    cout << "Begin Encoding..." << endl;
    MemoryController* bits = QuadrantTreeEncoder::encodeMatrix(matrix);
    QuadrantTreeEncoder::printAnalytics();
    //cout << bits->toStringAfter(QuadrantTreeEncoder::headerSize) << endl;
    Matrix<int>* decoded = QuadrantTreeEncoder::decodeMatrix<int>(bits);
    cout << "Decoded: " << endl;
    cout << decoded->toString() << endl;
    CachedTreeMatrix<int> treeMatrix(decoded);
    cout << treeMatrix.toString() << endl;
    for (size_t r = 0; r < treeMatrix.height(); r++) {
        for (size_t c = 0; c < treeMatrix.width(); c++) {
            int toSet = 0;
            cout << "SETTING (" << r << ", " << c << ") = " << toSet << endl;
            treeMatrix.set(r, c, toSet);
            cout << "RETURNED VAL = " << treeMatrix.get(r, c) << endl;
            cout << treeMatrix.toString() << endl;
        }
    }
    delete bits;
    delete matrix;
    delete decoded;
    while (true);
}
