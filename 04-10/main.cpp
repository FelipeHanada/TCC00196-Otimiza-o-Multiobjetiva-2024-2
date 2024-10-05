#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <vector>
#include "optimization.h"
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
        os << evl->get_evaluation(*s);
        os << " -> ";
    }
    
    long long evl_s1 = evl->get_evaluation(*s1);
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
    if (instance_name.compare("f6_l-d_kp_10_60") == 0) {
        std::cout << "instance name: " << instance_name << std::endl;
    }

    std::ofstream test_output_file(TEST_OUTPUT_DIR + std::string("/") + instance_name);
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
        test_output_file << "Optimum: " << optimum << std::endl;
        test_output_file << std::setw(100) << std::setfill('-') << "" << std::endl;
    }

    KnapsackEvaluator evl(n, q, v, w);

    std::map<std::string, KnapsackSolution (*)(const KnapsackEvaluator *evl)> cms = {
        {"Greedy", [](const KnapsackEvaluator *evl) -> KnapsackSolution { return cm_knapsack_greedy(evl); }},
        {"Random", [](const KnapsackEvaluator *evl) -> KnapsackSolution { return cm_knapsack_random(evl, 99999); }},
        {"Greedy Randomized", [](const KnapsackEvaluator *evl) -> KnapsackSolution { return cm_knapsack_greedy_randomized(evl, 99999, 0.1); }}
    };

    std::map<std::string, MovementGenerator<KnapsackSolution>*> mgs = {
        {"2 Flip Bit", new Knapsack2FlipBitMovementGenerator(&evl, n)},
        {"Interval Flip Bit", new KnapsackIntervalFlipBitMovementGenerator(&evl, n)},
        {"Interval Inversion", new KnapsackInversionMovementGenerator(&evl, n)}
    };

    for (auto &cm : cms) {
        KnapsackSolution s = cm.second(&evl);
        print_solution("Constructive Method: " + cm.first, &evl, &s, NULL, optimum, test_output_file);
        test_output_file << std::endl;

        KnapsackSolution s1(0);
        for (auto &mg : mgs) {
            RHFirstImprovement<KnapsackSolution> fi(&evl, mg.second);
            LSHillClimbing<KnapsackSolution> hill_climbing_fi(&evl, &fi);
            s1 = hill_climbing_fi.run(s, 10);
            print_solution(
                "Local Search: " + cm.first + " + Hill Climbing + First Improvement (" + mg.first + ")",
                &evl, &s1, &s, optimum, test_output_file
            );
            test_output_file << std::endl;

            RHBestImprovement<KnapsackSolution> bi(&evl, mg.second);
            LSHillClimbing<KnapsackSolution> hill_climbing_bi(&evl, &bi);
            s1 = hill_climbing_bi.run(s, 10);
            print_solution(
                "Local Search: " + cm.first + " + Hill Climbing + Best Improvement (" + mg.first + ")",
                &evl, &s1, &s, optimum, test_output_file
            );
            test_output_file << std::endl;

            RHRandomSelection<KnapsackSolution> rs(&evl, mg.second, 5);
            RandomDescentMethod<KnapsackSolution> random_descent(&evl, &rs, 10);
            s1 = random_descent.run(s, 10);
            print_solution(
                "Local Search: " + cm.first + " + Random Descent + Random Selection (" + mg.first + ")",
                &evl, &s1, &s, optimum, test_output_file
            );
            test_output_file << std::endl;

            MHSimulatedAnnealing<KnapsackSolution> simulated_annealing(
                &evl, mg.second, 100,
                0.95, 100000, 0.00001
            );
            s1 = simulated_annealing.run(s, 10);
            print_solution(
                "Meta Heuristic: " + cm.first + " + Simulated Annealing (" + mg.first + ")",
                &evl, &s1, &s, optimum, test_output_file
            );
        }

        test_output_file << std::setw(100) << std::setfill('-') << "" << std::endl;
    }

    for (auto &mg : mgs)
        delete mg.second;

    test_output_file.close();
}

int main() {
    for (const auto &entry : std::filesystem::directory_iterator(INSTANCE_DIR)) {
        if (!entry.is_regular_file()) continue;

        test_instance(entry.path().filename().string());
    }
}
