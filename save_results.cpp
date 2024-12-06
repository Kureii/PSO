/*
 * Created by kureii on 12/6/24.
*/

#include <fstream>
#include <format>
#include "save_results.h"
#include "pso.h"
#include "matplotlibcpp.h"
#include "test_functions.h"
#include "helpfull_structs.h"
#include "helpfull_functions.h"

namespace plt = matplotlibcpp;

std::string CreateParamSuffix(const PSOParams &params) {
    std::string suffix = "_";
    if (params.use_linear_inertia) {
        suffix += "linear_w" + std::to_string(params.w_start) + "_" + std::to_string(params.w_end);
    }
    else {
        suffix += "const_w" + std::to_string(params.w_start);
    }

    suffix += "_c1" + std::to_string(params.c1) + "_c2" + std::to_string(params.c2);
    suffix += params.global_topology ? "_global" : "_local";
    if (!params.global_topology) {
        suffix += "_n" + std::to_string(params.neighborhood_size);
    }

    for (auto &ch: suffix) {
        if (ch == '.') ch = 'd';
    }

    return suffix;
}

void DrawTable(const std::vector<TestFunction> &testFunctions,
               const std::map<std::string, std::vector<double> > &results_map,
               int runs,
               const PSOParams &params,
               const std::string &param_suffix) {
    static bool main_heading_printed = false;

    std::vector<std::string> metrics = {"Nejlepší", "Nejhorší", "Průměr", "Medián", "Směrodatná odchylka", "Graf"};
    std::map<std::string, std::map<std::string, double> > stats;

    for (auto &tf: testFunctions) {
        auto &vals = results_map.at(tf.name);
        double best_val = *std::ranges::min_element(vals);
        double worst_val = *std::ranges::max_element(vals);
        double avg_val = Mean(vals);
        double med_val = Median(vals);
        double std_val = Stddev(vals);

        stats["Nejlepší"][tf.name] = best_val;
        stats["Nejhorší"][tf.name] = worst_val;
        stats["Průměr"][tf.name] = avg_val;
        stats["Medián"][tf.name] = med_val;
        stats["Směrodatná odchylka"][tf.name] = std_val;
        stats["Graf"][tf.name] = 0.0;
    }

    std::ofstream ofs("results.md", std::ios::app);

    if (!main_heading_printed) {
        ofs << "# Výsledky experimentů\n\n";
        main_heading_printed = true;
    }

    ofs << "## Parametry experimentu\n\n";
    ofs << "| Parametr                 | Hodnota                    |\n";
    ofs << "|--------------------------|----------------------------|\n";

    if (params.use_linear_inertia) {
        ofs << "| Setrvačnost (w)          | " << params.w_start << " → " << params.w_end << " |\n";
    }
    else {
        ofs << "| Setrvačnost (w)          | " << params.w_start << " (konstantní) |\n";
    }

    ofs << "| c1                       | " << params.c1 << " |\n";
    ofs << "| c2                       | " << params.c2 << " |\n";

    std::string topologie = params.global_topology ? "Globální (hvězda)" : "Kruhová (lokální)";
    ofs << "| Topologie                 | " << topologie << " |\n";

    ofs << "| Použita lineární setrvačnost | " << (params.use_linear_inertia ? "Ano" : "Ne") << " |\n";

    if (!params.global_topology) {
        ofs << "| Velikost sousedství       | " << params.neighborhood_size << " |\n";
    }
    else {
        ofs << "| Velikost sousedství       | N/A (globální topologie) |\n";
    }

    ofs << "| Rozsah dimenze (D)        | " << params.D << " |\n";
    ofs << "| Počet částic (NP)         | " << params.NP << " |\n";
    ofs << "| MAXFES                     | " << params.MAXFES << " |\n";
    ofs << "| Počet běhů (runs)         | " << runs << " |\n\n";

    ofs << "Níže uvedena jest tabulka s výsledky po " << runs << " bězích pro jednotlivé funkce:\n\n";

    ofs << "| Metrika               |";
    for (auto &tf: testFunctions) {
        ofs << " " << tf.name << " |";
    }
    ofs << "\n|-----------------------|";
    for (size_t i = 0; i < testFunctions.size(); i++) ofs << "----------|";
    ofs << "\n";

    for (auto &m: metrics) {
        ofs << "| " << m << " |";
        for (auto &tf: testFunctions) {
            if (m == "Graf") {
                ofs << " ![" << tf.name << "](" << tf.name << param_suffix << "_convergence.png) |";
            }
            else {
                ofs << " " << stats[m][tf.name] << " |";
            }
        }
        ofs << "\n";
    }

    ofs << "\n\n";
    ofs.close();
}

void DrawGraphs(TestFunction &tf, const std::vector<std::vector<double> > &all_convergences, int runs,
                const std::string &param_suffix) {
    size_t max_len = 0;
    for (auto &c: all_convergences) {
        if (c.size() > max_len) max_len = c.size();
    }

    std::vector<double> mean_curve(max_len, 0.0);
    for (auto &c: all_convergences) {
        for (size_t i = 0; i < c.size(); i++) {
            mean_curve[i] += c[i];
        }
    }
    for (size_t i = 0; i < mean_curve.size(); i++) {
        mean_curve[i] /= (double) runs;
    }

    plt::figure();
    for (auto &c: all_convergences) {
        plt::plot(c, {{"color", "gray"}, {"linestyle", "--"}});
    }

    plt::plot(mean_curve, {{"color", "red"}, {"linewidth", "2"}, {"label", "Průměr"}});
    plt::title("Konvergenční křivka - " + tf.name);
    plt::xlabel("Iterace");
    plt::ylabel("Nejlepší nalezené fitness");
    plt::legend();
    plt::grid(true);

    plt::save(tf.name + param_suffix + "_convergence.png");
}
