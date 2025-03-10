#include <gtest/gtest.h>
#include <cuComplex.h>
#include "../functionality/fftShift.hpp"

TEST(fftShift, fftShiftVector1DEven)
{
    std::vector<double> data1D = {0, 1, 2, 3, 4, 5, 6, 7};
    fftshift1D(data1D);
    EXPECT_TRUE((data1D == std::vector<double>{4, 5, 6, 7, 0, 1, 2, 3}));
}

TEST(fftShift, fftShiftVector1DOdd)
{
    std::vector<double> data1D = {0, 1, 2, 3, 4, 5, 6};
    fftshift1D(data1D);
    EXPECT_TRUE((data1D == std::vector<double>{4, 5, 6, 0, 1, 2, 3}));
}

TEST(fftShift, fftShiftCArray1DEven)
{
    double data1D[] = {0, 1, 2, 3, 4, 5, 6, 7};
    int n = sizeof(data1D) / sizeof(data1D[0]);
    fftshift1D(data1D, n);
    double expected1[] = {4, 5, 6, 7, 0, 1, 2, 3};
    for (int i = 0; i < n; ++i)
        EXPECT_EQ(data1D[i], expected1[i]);
}

TEST(fftShift, fftShiftCArray1DOdd)
{
    double data2D[] = {0, 1, 2, 3, 4, 5, 6};
    int n = sizeof(data2D) / sizeof(data2D[0]);
    fftshift1D(data2D, n);
    double expected2[] = {4, 5, 6, 0, 1, 2, 3};
    for (int i = 0; i < n; ++i)
        EXPECT_EQ(data2D[i], expected2[i]);
}

TEST(fftShift, fftShiftCuDoubleComplex1DEven)
{
    cuDoubleComplex data1D[] = {
        make_cuDoubleComplex(0.0, 0.0), make_cuDoubleComplex(1.0, 1.0),
        make_cuDoubleComplex(2.0, 2.0), make_cuDoubleComplex(3.0, 3.0),
        make_cuDoubleComplex(4.0, 4.0), make_cuDoubleComplex(5.0, 5.0),
        make_cuDoubleComplex(6.0, 6.0), make_cuDoubleComplex(7.0, 7.0)};
    int n = sizeof(data1D) / sizeof(data1D[0]);
    fftshift1D(data1D, n);
    cuDoubleComplex expected1[] = {
        make_cuDoubleComplex(4.0, 4.0), make_cuDoubleComplex(5.0, 5.0),
        make_cuDoubleComplex(6.0, 6.0), make_cuDoubleComplex(7.0, 7.0),
        make_cuDoubleComplex(0.0, 0.0), make_cuDoubleComplex(1.0, 1.0),
        make_cuDoubleComplex(2.0, 2.0), make_cuDoubleComplex(3.0, 3.0)};
    for (int i = 0; i < n; ++i)
    {
        EXPECT_EQ(cuCreal(data1D[i]), cuCreal(expected1[i]));
        EXPECT_EQ(cuCimag(data1D[i]), cuCimag(expected1[i]));
    }
}

TEST(fftShift, fftShiftCuDoubleComplex1DOdd)
{
    cuDoubleComplex data2D[] = {
        make_cuDoubleComplex(0.0, 0.0), make_cuDoubleComplex(1.0, 1.0),
        make_cuDoubleComplex(2.0, 2.0), make_cuDoubleComplex(3.0, 3.0),
        make_cuDoubleComplex(4.0, 4.0), make_cuDoubleComplex(5.0, 5.0),
        make_cuDoubleComplex(6.0, 6.0)};
    int n = sizeof(data2D) / sizeof(data2D[0]);
    fftshift1D(data2D, n);
    cuDoubleComplex expected2[] = {
        make_cuDoubleComplex(4.0, 4.0), make_cuDoubleComplex(5.0, 5.0),
        make_cuDoubleComplex(6.0, 6.0), make_cuDoubleComplex(0.0, 0.0),
        make_cuDoubleComplex(1.0, 1.0), make_cuDoubleComplex(2.0, 2.0),
        make_cuDoubleComplex(3.0, 3.0)};
    for (int i = 0; i < n; ++i)
    {
        EXPECT_EQ(cuCreal(data2D[i]), cuCreal(expected2[i]));
        EXPECT_EQ(cuCimag(data2D[i]), cuCimag(expected2[i]));
    }
}

TEST(FFTShift2DTest, BasicShift)
{
    const int rows = 4, cols = 4;
    cuDoubleComplex data[rows * cols] = {
        {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}};

    cuDoubleComplex expected[rows * cols] = {
        {11, 0}, {12, 0}, {9, 0}, {10, 0}, {15, 0}, {16, 0}, {13, 0}, {14, 0}, {3, 0}, {4, 0}, {1, 0}, {2, 0}, {7, 0}, {8, 0}, {5, 0}, {6, 0}};

    // Apply FFTShift 2D
    fftshift2D(data, rows, cols);

    // Verify the result

    for (int i = 0; i < rows * cols; ++i)
    {
        EXPECT_EQ(data[i].x, expected[i].x);
        EXPECT_EQ(data[i].y, expected[i].y);
    }
}