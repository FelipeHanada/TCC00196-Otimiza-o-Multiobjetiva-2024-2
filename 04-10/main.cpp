#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <vector>
#include "optimization.hpp"
#include "knapsack.h"
#include "neighborhood_exploration.h"
#include "meta_heuristics.h"

#define INSTANCE_DIR "./tests/instances-low_dimensional"
#define OPTIMUM_DIR "./tests/optimum-low_dimensional"
#define TEST_OUTPUT_DIR "./tests/output"

void print_solution(
    std::string label,
    KnapsackEvaluator *evl,
    KnapsackSolution *s1,
    KnapsackSolution *s = nullptr,
    long long optimum = -1,
    std::ostream &os = std::cout
) {
    os << label << std::endl;

    os << "Evaluation: ";
    if (s != nullptr) {
        os << evl->get_evaluation(s) << " -> ";
    }
    
    long long evl_s1 = evl->get_evaluation(s1);
    os << evl_s1;
    if (optimum != -1) {
        long long delta = optimum - evl_s1;
        os << " (" << std::setprecision(2) << (delta / (double) optimum * 100) << "% from optimum)";
    }
    
    os << " / ";
    
    for (int i=0; i<s1->size(); i++) {
        if (s1->get(i))
            os << i << " ";
    }
    os << std::endl;
}

void test_instance(std::string instance_name) {
    std::cout << "Testing instance: " << instance_name << std::endl;

    std::ofstream test_output_file(TEST_OUTPUT_DIR + std::string("/") + instance_name + std::string(".txt"));
    if (!test_output_file.is_open()) {
        std::cerr << "Error opening test output file: " << instance_name << std::endl;
        return;
    }

    std::ifstream instance_file(INSTANCE_DIR + std::string("/") + instance_name);
    if (!instance_file.is_open()) {
        std::cerr << "Error opening instance file: " << instance_name << std::endl;
        return;
    }

    int n, q;
    instance_file >> n >> q;

    std::vector<int> v(n), w(n);
    for (int i = 0; i < n; ++i) {
        instance_file >> v[i];
    }
    for (int i = 0; i < n; ++i) {
        instance_file >> w[i];
    }

    instance_file.close();

    std::ifstream optimum_file(OPTIMUM_DIR + std::string("/") + instance_name);
    long long optimum;
    if (!optimum_file.is_open()) {
        optimum = -1;
    } else {
        optimum_file >> optimum;
        optimum_file.close();
    }

    if (optimum != -1) {
        test_output_file << "Optimum: " << optimum << std::endl
            << std::endl;
        test_output_file << std::setw(100) << std::setfill('-') << "" << std::endl;
    }

    KnapsackEvaluator evl(n, q, v, w);
    MovementGenerator<KnapsackSolution> *mg = new KnapsackInversionMovementGenerator(&evl, n);

    KnapsackSolution* s1;

    RHRandomSelection<KnapsackSolution> rs(&evl, mg, 10000);
    LSHillClimbing<KnapsackSolution> hill_climbing(&evl, &rs);
    MHGrasp<KnapsackSolution> grasp(&evl,
        [](Evaluator<KnapsackSolution> *evl, double alpha) -> KnapsackSolution* {
            return cm_knapsack_greedy_randomized((KnapsackEvaluator*) evl, alpha, 5); 
        },
        0.5, &hill_climbing, 1000
    );
    s1 = grasp.run(600);
    print_solution(
        "Meta Heuristic: GRASP",
        &evl, s1, NULL, optimum, test_output_file
    );
    delete s1;
    test_output_file << std::endl;

    test_output_file << std::setw(100) << std::setfill('-') << "" << std::endl;

    KnapsackSolution* s = cm_knapsack_greedy_randomized(&evl, 0.9, 99999);
    print_solution("Constructive Method: Greedy Randomized", &evl, s, NULL, optimum, test_output_file);
    test_output_file << std::endl;
    MHSimulatedAnnealing<KnapsackSolution> simulated_annealing(
        &evl, mg, s, 100000,
        0.95, 1.05, 0.9, 0.00001
    );
    s1 = simulated_annealing.run(600);
    print_solution(
        "Meta Heuristic: Simulated Annealing",
        &evl, s1, s, optimum, test_output_file
    );
    delete s1;
    test_output_file << std::endl;

    delete mg;
    test_output_file.close();
}

int main() {
    for (const auto &entry : std::filesystem::directory_iterator(INSTANCE_DIR)) {
        if (!entry.is_regular_file()) continue;

        test_instance(entry.path().filename().string());
    }
}
