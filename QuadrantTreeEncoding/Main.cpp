#include "Library.h"
#include "Tests.h"
#include "MemoryController.h"
#include "QuadrantTreeEncoder.h"
#include "BitList.h"

int main()
{
    BitList bitList(64);
    bitList.set(10, true);
    bool bVal = bitList.get(4);
    cout << "Returned: " << bVal << endl;
    bitList.set<byte>(27, byte{ 255 });
    byte* value = bitList.get<byte>(27);
    cout << "INT VAL: " << to_integer<int>(*value) << endl;
    cout << bitList.toString() << endl;
    auto valuePicker = [](size_t r, size_t c)->int {
        return rand()%127;
    };
    auto pointPicker = [](size_t num, vector<size_t>& points) {
        size_t index = rand() % points.size();
        size_t point = points[index];
        points.erase(points.begin() + index, points.begin() + index + 1);
        return point;
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
    Matrix<int>* matrix = generateMatrix<int>(r,c,fullness,pointPicker,valuePicker,0);
    //printMatrix(matrix);
    MemoryController* bits = QuadrantTreeEncoder::encodeMatrix(matrix, intEncoder<int,8>);
    QuadrantTreeEncoder::printAnalytics();
    cout << bits->toStringAfter(QuadrantTreeEncoder::headerSize) << endl;
    Matrix<int>* decoded = QuadrantTreeEncoder::decodeMatrix(bits, intDecoder<int, 8>);
    //printMatrix(decoded);
    delete bits;
    delete matrix;
    delete decoded;
    while (true);
}
