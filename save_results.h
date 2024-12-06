/*
 * Created by kureii on 12/6/24.
*/

#pragma once
#include <vector>
#include <map>
#include "pso_structs.h"
#include "helpfull_structs.h"

extern std::string CreateParamSuffix(const PSOParams &params);

extern void DrawTable(const std::vector<TestFunction> &testFunctions,
                      const std::map<std::string, std::vector<double> > &results_map,
                      int runs,
                      const PSOParams &params,
                      const std::string &param_suffix);

extern void DrawGraphs(TestFunction &tf, const std::vector<std::vector<double> > &all_convergences, int runs,
                       const std::string &param_suffix);
