#include <functional>
#include "Functions.cpp"
#include "MemoryController.cpp"

using namespace std;

class MatrixEncoder {

public:

    virtual void printAnalytics();

    template<typename V, size_t N>
    MemoryController encodeMatrix(BitEncoder<V, N> encoder);

    template<typename V, size_t N>
    V** decodeMatrix(BitDecoder<N, V> decoder);
};