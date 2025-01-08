#include <vector>
#include <algorithm>
#include "fftShift.hpp"

void fftshift1D(std::vector<double>& data) {
    int n = data.size();
    int half = n / 2;
    if (n % 2 == 0) {
        std::rotate(data.begin(), data.begin() + half, data.end());
    } else {
        std::rotate(data.begin(), data.begin() + half + 1, data.end());
    }
}


void fftshift1D(double* data, int length) {
    int half = length / 2;
    if (length % 2 == 0) {
        std::rotate(data, data + half, data + length);
    } else {
        std::rotate(data, data + half + 1, data + length);
    }
}