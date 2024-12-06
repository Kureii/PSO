/*
 * Created by kureii on 12/6/24.
*/

#include <cmath>
#include <algorithm>
#include "helpfull_functions.h"

double Median(std::vector<double> v) {
    std::ranges::sort(v);
    if (const size_t n = v.size(); n % 2 == 0)
        return 0.5 * (v[n / 2 - 1] + v[n / 2]);
    else
        return v[n / 2];
}

double Mean(const std::vector<double> &v) {
    double s = 0.0;
    for (auto &x: v) s += x;
    return s / static_cast<double>(v.size());
}

double Stddev(const std::vector<double> &v) {
    const double m = Mean(v);
    double s = 0.0;
    for (auto &x: v) {
        const double diff = x - m;
        s += diff * diff;
    }
    s /= static_cast<double>(v.size());
    return std::sqrt(s);
}
