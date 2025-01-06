#include <iostream>
#include <cuComplex.h>
#include "functionality/WriteAdjMat.hpp"
#include "functionality/GetAdjMat.hpp"
#include "functionality/Utilities.hpp"
#include <vector>

int main()
{
    int evenqubits = 6;
    int len = 1 << evenqubits;
    auto rowOffset = std::vector<int>(getRowOffsetSizeMini(evenqubits), 0);
    auto columnIndices = std::vector<int>(getColumnIndexSizeMini(evenqubits), 0);
    cuDoubleComplex *values = new cuDoubleComplex[getValuesSizeMini(evenqubits)];

    int rowOffsetSize;
    int columnIndicesSize;
    int valuesSize;

    writeMatAMiniCSR(rowOffset.data(),
                     columnIndices.data(),
                     values,
                     evenqubits,
                     rowOffsetSize,
                     columnIndicesSize,
                     valuesSize);

    for (int i = 0; i < rowOffsetSize; ++i)
    {
        std::cout << rowOffset[i] << " , ";
    }
    std::cout << std::endl;

    for (int i = 0; i < columnIndicesSize; ++i)
    {
        std::cout << columnIndices[i] << " , ";
    }
    std::cout << std::endl;

    for (int i = 0; i < valuesSize; ++i)
    {
        std::cout << cuCreal(values[i]) << " , ";
    }
    std::cout << std::endl;

    // auto matA = getMatAMini(evenqubits);

    // auto matB = csrToDense(values, rowOffset, columnIndices, len, len);

    // for (int i = 0; i < len; ++i)
    // {
    //     for (int j = 0; j < len; ++j)
    //     {
    //         if(matA[i][j].real() - matB[i][j] > 0.003){
    //             std::cout << "failed" << matA[i][j] << " , " << matB[i][j];
    //         }
    //     }
    // }

    // delete[] values;

    return 0;
}
