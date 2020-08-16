#include "Library.h"
#include "Tests.h"
#include "MemoryController.h"
#include "QuadrantTreeEncoder.h"
#include "BitList.h"

int main()
{
    auto valuePicker = [](size_t r, size_t c)->int {
        return (rand()%127) + 1;
    };
    auto pointPicker = [](size_t num, vector<size_t>& points) {
        size_t index = rand() % points.size();
        size_t point = points[index];
        points.erase(points.begin() + index, points.begin() + index + 1);
        return point;
    };
    auto generator = [](Matrix<int>* matrix) {

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
    cout << bits->toStringAfter(QuadrantTreeEncoder::headerSize) << endl;
    Matrix<int>* decoded = QuadrantTreeEncoder::decodeMatrix<int>(bits);
    cout << "Decoded: " << endl;
    //printMatrix(decoded);
    delete bits;
    delete matrix;
    delete decoded;
    while (true);
}
