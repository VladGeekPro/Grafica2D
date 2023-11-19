#pragma once

#include "pch.h"
#include <iostream>
#include <cmath>

double ln1MinusX(double x, double epsilon) {
    if (x >= 1 || x < -1) {
        std::cerr << "Значение x должно находиться в интервале (-1, 1)" << std::endl;
        return 0.0;
    }

    double result = 0.0;
    int k = 1;

    while (true) {
        double term = std::pow(x, k) / k;
        if (std::abs(term) < epsilon)
            break;

        result -= term;
        k++;
    }

    return result;
}
