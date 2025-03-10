/*
 * Copyright (c) 2021-2023, NVIDIA CORPORATION & AFFILIATES.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <cuda_runtime_api.h> // cudaMalloc, cudaMemcpy, etc.
#include <cuComplex.h>        // cuDoubleComplex
#include <custatevec.h>       // custatevecApplyMatrix
#include <stdio.h>            // printf
#include <stdlib.h>           // EXIT_FAILURE

#include "../CudaControl/Helper.hpp"         // HANDLE_ERROR, HANDLE_CUDA_ERROR

int main(void) {

    const int nIndexBits = 3;
    const int nSvSize    = (1 << nIndexBits);
    const int nMaxShots  = 5;
    const int nShots     = 5;

    const int bitStringLen  = 2;
    const int bitOrdering[] = {0, 1};

    custatevecIndex_t bitStrings[nShots];
    custatevecIndex_t bitStrings_result[] = {0b00, 0b01, 0b10, 0b11, 0b11};

    cuDoubleComplex h_sv[]        = {{ 0.0, 0.0}, { 0.0, 0.1}, { 0.1, 0.1}, { 0.1, 0.2}, 
                                     { 0.2, 0.2}, { 0.3, 0.3}, { 0.3, 0.4}, { 0.4, 0.5}};

    // In real appliction, random numbers in range [0, 1) will be used.
    const double randnums[] = {0.1, 0.8, 0.4, 0.6, 0.2};

    custatevecSamplerDescriptor_t sampler;

    cuDoubleComplex *d_sv;
    CHECK_CUDA( cudaMalloc((void**)&d_sv, nSvSize * sizeof(cuDoubleComplex)) );

    CHECK_CUDA( cudaMemcpy(d_sv, h_sv, nSvSize * sizeof(cuDoubleComplex), 
                       cudaMemcpyHostToDevice) );

    //----------------------------------------------------------------------------------------------

    // custatevec handle initialization
    custatevecHandle_t handle;
    CHECK_CUSTATEVECTOR( custatevecCreate(&handle) );

    void* extraWorkspace = nullptr;
    size_t extraWorkspaceSizeInBytes = 0;

    // create sampler and check the size of external workspace
    CHECK_CUSTATEVECTOR( custatevecSamplerCreate(
                  handle, d_sv, CUDA_C_64F, nIndexBits, &sampler, nMaxShots, 
                  &extraWorkspaceSizeInBytes) );
    
    // allocate external workspace if necessary
    if (extraWorkspaceSizeInBytes > 0)
        CHECK_CUDA( cudaMalloc(&extraWorkspace, extraWorkspaceSizeInBytes) );
    
    // sample preprocess
    CHECK_CUSTATEVECTOR( custatevecSamplerPreprocess(
                  handle, sampler, extraWorkspace, extraWorkspaceSizeInBytes) );
    
    // sample bit strings
    CHECK_CUSTATEVECTOR( custatevecSamplerSample(
                  handle, sampler, bitStrings, bitOrdering, bitStringLen, randnums, nShots, 
                  CUSTATEVEC_SAMPLER_OUTPUT_ASCENDING_ORDER) );

    // destroy descriptor and handle
    CHECK_CUSTATEVECTOR( custatevecSamplerDestroy(sampler) );
    CHECK_CUSTATEVECTOR( custatevecDestroy(handle) );

    //----------------------------------------------------------------------------------------------

    CHECK_CUDA( cudaMemcpy(h_sv, d_sv, nSvSize * sizeof(cuDoubleComplex),
                       cudaMemcpyDeviceToHost) );

    bool correct = true;
    for (int i = 0; i < nShots; i++) {
        if (bitStrings[i] != bitStrings_result[i]) {
            correct = false;
            break;
        }
    }

    CHECK_CUDA( cudaFree(d_sv) );
    if (extraWorkspaceSizeInBytes)
        CHECK_CUDA( cudaFree(extraWorkspace) );

    if (correct) {
        printf("sampler example PASSED\n");
        return EXIT_SUCCESS;
    }
    else {
        printf("sampler example FAILED: wrong result\n");
        return EXIT_FAILURE;
    }
}