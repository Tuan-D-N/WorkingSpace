#pragma once
#include <cuda_runtime_api.h>
#include <cuComplex.h>
#include <custatevec.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <array>
#include <vector>
#include "../CudaControl/Helper.hpp"

/// @brief The sampling system
/// @param handle custatevecHandle object
/// @param nIndexBits number of qubits
/// @param bitOrdering cstyle array; the order of the bits to sample in the result
/// @param bitStringLen the length of of the bitOrdering array.
/// @param d_sv statevector on device
/// @param bitStrings_out cstyle array; output result length "nshots"
/// @param nShots number of shots to run
/// @param extraWorkspace extra workspace variable
/// @param extraWorkspaceSizeInBytes the size of extra workspace
/// @return error value
int sampleSV(custatevecHandle_t &handle,
             const int nIndexBits,
             const int bitOrdering[], // Qubits to measure
             const int bitStringLen,  // length of bitOrdering
             cuDoubleComplex d_sv[],
             custatevecIndex_t bitStrings_out[],
             const int nShots,
             void *extraWorkspace,
             size_t &extraWorkspaceSizeInBytes,
             double randnums[] = nullptr);

int sampleSV(custatevecHandle_t &handle,
             const int nIndexBits,
             const std::vector<int> &bitOrdering, // Qubits to measure
             cuDoubleComplex d_sv[],
             custatevecIndex_t bitStrings_out[],
             int nShots,
             void *extraWorkspace,
             size_t &extraWorkspaceSizeInBytes,
             double randnums[] = nullptr);

template <int bitStringLen>
int sampleSV(custatevecHandle_t &handle,
             const int nIndexBits,
             const std::array<int, bitStringLen> &bitOrdering, // Qubits to measure
             cuDoubleComplex d_sv[],
             custatevecIndex_t bitStrings_out[],
             const int nShots,
             void *extraWorkspace,
             size_t &extraWorkspaceSizeInBytes,
             double randnums[] = nullptr)
{
    CHECK_BROAD_ERROR(
        sampleSV(
            handle,
            nIndexBits,
            bitOrdering.data(),
            bitOrdering.size(),
            d_sv,
            bitStrings_out,
            nShots,
            extraWorkspace,
            extraWorkspaceSizeInBytes,
            randnums));
    return cudaSuccess;
}