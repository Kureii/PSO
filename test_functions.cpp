/*
 * Created by kureii on 12/6/24.
*/

#include "test_functions.h"
#include <cmath>
#include <iostream>

double SphereFunction(const std::vector<double> &x) {
    double sum = 0.0;
    for (auto xi: x) sum += xi * xi;
    return sum;
}

double SchwefelFunction(const std::vector<double> &x) {
    double sum = 0.0;
    for (auto xi: x) {
        sum += (-xi) * std::sin(std::sqrt(std::fabs(xi)));
    }
    double d = (double) x.size();
    return 418.9829 * d - sum;
}

double RosenbrockFunction(const std::vector<double> &x) {
    double sum = 0.0;
    for (size_t i = 0; i < x.size() - 1; i++) {
        double term1 = x[i + 1] - x[i] * x[i];
        double term2 = x[i] - 1.0;
        sum += 100.0 * term1 * term1 + term2 * term2;
    }
    return sum;
}
