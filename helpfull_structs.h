/*
 * Created by kureii on 12/6/24.
*/

#pragma once
#include <vector>
#include <string>

struct TestFunction {
    std::string name;

    double (*func)(const std::vector<double> &);

    double lb;
    double ub;
};
