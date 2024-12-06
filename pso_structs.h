/*
 * Created by kureii on 12/6/24.
*/

#pragma once

#include <vector>

struct Particle {
    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> best_position;
    double best_fitness;
};

struct PSOParams {
    int D;
    int NP;
    int MAXFES;
    double w;
    double w_start;
    double w_end;
    double c1;
    double c2;
    double lower_bound;
    double upper_bound;
    bool use_linear_inertia;
    bool global_topology;
    int neighborhood_size;
};
