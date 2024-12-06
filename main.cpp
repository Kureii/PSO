#include <vector>
#include <random>
#include "pso.h"
#include "test_functions.h"
#include "helpfull_structs.h"
#include "save_results.h"

int main() {
    std::vector<PSOParams> params_variants;

    PSOParams p{};
    p.D = 30;
    p.NP = 50;
    p.MAXFES = 10000;
    p.w_start = 0.8;
    p.w_end = 0.3;
    p.c1 = 2.0;
    p.c2 = 2.0;
    p.use_linear_inertia = true;
    p.global_topology = true;
    p.neighborhood_size = 2;
    params_variants.push_back(p);

    p.D = 30;
    p.NP = 50;
    p.MAXFES = 10000;
    p.w_start = 0.7;
    p.w_end = 0.7;
    p.c1 = 1.49618;
    p.c2 = 1.49618;
    p.use_linear_inertia = false;
    p.global_topology = true;
    p.neighborhood_size = 2;
    params_variants.push_back(p);

    p.D = 30;
    p.NP = 50;
    p.MAXFES = 10000;
    p.w_start = 0.6;
    p.w_end = 0.6;
    p.c1 = 1.49618;
    p.c2 = 1.49618;
    p.use_linear_inertia = false;
    p.global_topology = false;
    p.neighborhood_size = 2;
    params_variants.push_back(p);


    std::vector<TestFunction> test_functions = {
        {"Sphere", SphereFunction, -5.0, 5.0},
        {"Schwefel", SchwefelFunction, -500.0, 500.0},
        {"Rosenbrock", RosenbrockFunction, -2.0, 2.0}
    };

    int runs = 100;

    for (size_t i = 0; i < params_variants.size(); i++) {
        auto &params = params_variants[i];
        auto param_sufix = CreateParamSuffix(params);

        std::map<std::string, std::vector<double> > results_map;
        std::map<std::string, std::vector<std::vector<double> > > conv_map;
        for (auto &tf: test_functions) {
            params.lower_bound = tf.lb;
            params.upper_bound = tf.ub;

            std::vector<double> final_results;
            std::vector<std::vector<double> > all_convergences;

            for (int r = 0; r < runs; r++) {
                PSO pso(params, tf.func);
                auto conv = pso.Run();
                double best = pso.GetGlobalBestFitness();
                final_results.push_back(best);
                all_convergences.push_back(conv);
            }

            results_map[tf.name] = final_results;
            conv_map[tf.name] = all_convergences;
            DrawGraphs(tf, all_convergences, runs, param_sufix);
        }

        DrawTable(test_functions, results_map, runs, params, param_sufix);
    }

    return 0;
}
