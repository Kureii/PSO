/*
 * Created by kureii on 12/6/24.
*/
#include <limits>
#include "pso.h"

#include <iostream>

PSO::PSO(const PSOParams &params, double (*fitness_func_)(const std::vector<double> &))
    : p_(params), gen_(rd_()), dist_(0.0, 1.0), fitness_func_(fitness_func_) {
    Init();
}

std::vector<double> PSO::Run() {
    int FES = 0;
    std::vector<double> convergen_ce;
    convergen_ce.reserve(p_.MAXFES / p_.NP + 1);

    int iterations = p_.MAXFES / p_.NP;
    for (int it = 0; it < iterations; it++) {
        double w_current = p_.use_linear_inertia
                               ? p_.w_start - (p_.w_start - p_.w_end) * (double) (it * p_.NP) / (double) p_.MAXFES
                               : p_.w;

        for (int i = 0; i < p_.NP; i++) {
            std::vector<double> best_neighbor_pos = GetNeighborhoodBest(i);
            for (int d = 0; d < p_.D; d++) {
                double r1 = dist_(gen_);
                double r2 = dist_(gen_);
                double vel = swarm_[i].velocity[d];
                double cognitive = p_.c1 * r1 * (swarm_[i].best_position[d] - swarm_[i].position[d]);
                double social = p_.c2 * r2 * (best_neighbor_pos[d] - swarm_[i].position[d]);
                double vnew = w_current * vel + cognitive + social;

                double vmax = 0.2 * (p_.upper_bound - p_.lower_bound);
                if (vnew > vmax) vnew = vmax;
                if (vnew < -vmax) vnew = -vmax;

                double new_pos = swarm_[i].position[d] + vnew;
                if (new_pos > p_.upper_bound) new_pos = p_.upper_bound;
                if (new_pos < p_.lower_bound) new_pos = p_.lower_bound;

                swarm_[i].velocity[d] = vnew;
                swarm_[i].position[d] = new_pos;
            }

            double f = fitness_func_(swarm_[i].position);
            FES++;
            if (f < swarm_[i].best_fitness) {
                swarm_[i].best_fitness = f;
                swarm_[i].best_position = swarm_[i].position;
                if (f < global_best_fitness_) {
                    global_best_fitness_ = f;
                    global_best_position_ = swarm_[i].best_position;
                }
            }

            if (FES >= p_.MAXFES) break;
        }

        convergen_ce.push_back(global_best_fitness_);
        if (FES >= p_.MAXFES) break;
    }

    return convergen_ce;
}

double PSO::GetGlobalBestFitness() const {
    return global_best_fitness_;
}


void PSO::Init() {
    swarm_.resize(p_.NP);
    for (int i = 0; i < p_.NP; i++) {
        swarm_[i].position.resize(p_.D);
        swarm_[i].velocity.resize(p_.D);
        swarm_[i].best_position.resize(p_.D);
        for (int d = 0; d < p_.D; d++) {
            double x = p_.lower_bound + (p_.upper_bound - p_.lower_bound) * dist_(gen_);
            double v = 0.0;
            swarm_[i].position[d] = x;
            swarm_[i].velocity[d] = v;
        }
        double f = fitness_func_(swarm_[i].position);
        swarm_[i].best_fitness = f;
        swarm_[i].best_position = swarm_[i].position;
    }

    global_best_fitness_ = std::numeric_limits<double>::infinity();
    for (int i = 0; i < p_.NP; i++) {
        if (swarm_[i].best_fitness < global_best_fitness_) {
            global_best_fitness_ = swarm_[i].best_fitness;
            global_best_position_ = swarm_[i].best_position;
        }
    }
}

std::vector<double> PSO::GetNeighborhoodBest(int idx) {
    if (p_.global_topology) {
        return global_best_position_;
    }
    int half = p_.neighborhood_size / 2;
    double best_local_fit = std::numeric_limits<double>::infinity();
    std::vector<double> best_local_pos(p_.D);
    for (int offset = -half; offset <= half; offset++) {
        int neighbor = idx + offset;
        if (neighbor < 0) neighbor += p_.NP;
        if (neighbor >= p_.NP) neighbor -= p_.NP;
        if (swarm_[neighbor].best_fitness < best_local_fit) {
            best_local_fit = swarm_[neighbor].best_fitness;
            best_local_pos = swarm_[neighbor].best_position;
        }
    }
    return best_local_pos;
}
