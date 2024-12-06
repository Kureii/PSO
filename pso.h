/*
 * Created by kureii on 12/6/24.
 */

#pragma once

#include <vector>
#include <random>

#include "pso_structs.h"

class PSO {
public:
    PSO(const PSOParams &params, double (*fitness_func)(const std::vector<double> &));

    std::vector<double> Run();

    [[nodiscard]] double GetGlobalBestFitness() const;

private:
    PSOParams p_;
    std::vector<Particle> swarm_;
    double global_best_fitness_;
    std::vector<double> global_best_position_;

    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_real_distribution<> dist_;

    double (*fitness_func_)(const std::vector<double> &);

    void Init();

    std::vector<double> GetNeighborhoodBest(int idx);
};
