#include <iostream>
#include <vector>
#include <iomanip>
#include "optimization.h"
#include "knapsack.h"
#include "neighborhood_exploration.h"
#include "meta_heuristics.h"

void print_solution(std::string label, KnapsackEvaluator *evl, KnapsackSolution *s1, KnapsackSolution *s = nullptr) {
    std::cout << label << std::endl;

    std::cout << "Evaluation: ";
    if (s != nullptr) {
        std::cout << evl->get_evaluation(*s);
        std::cout << " -> ";
    }
    std::cout << evl->get_evaluation(*s1) << " / ";
    for (int i=0; i<s1->size(); i++) {
        if (s1->get(i))
            std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main() {
    freopen("knapsack-input.txt", "r", stdin);
    freopen("knapsack-output.txt", "w", stdout);

    int t;
    std::cin >> t;
    for (int test=0; test<t; test++) {
        std::cout << "[ " << std::setw(45) << std::setfill('-') << "" << " TEST " << test << " " << std::setw(45) << std::setfill('-') << " ]" << std::endl;
        int n, q;
        std::cin >> n >> q;

        std::vector<int> v, w;
        for (int i=0; i<n; i++) {
            int x; std::cin >> x;
            v.push_back(x);
        }
        for (int i=0; i<n; i++) {
            int x; std::cin >> x;
            w.push_back(x);
        }

        KnapsackEvaluator evl(n, q, v, w);

        std::map<std::string, KnapsackSolution (*)(const KnapsackEvaluator *evl)> cms = {
            {"Greedy", [](const KnapsackEvaluator *evl) -> KnapsackSolution { return cm_knapsack_greedy(evl); }},
            {"Random", [](const KnapsackEvaluator *evl) -> KnapsackSolution { return cm_knapsack_random(evl, 10); }},
            {"Greedy Randomized", [](const KnapsackEvaluator *evl) -> KnapsackSolution { return cm_knapsack_greedy_randomized(evl, 10, 0.1); }}
        };

        std::map<std::string, MovementGenerator<KnapsackSolution>*> mgs = {
            {"2 Flip Bit", new Knapsack2FlipBitMovementGenerator(&evl, n)},
            {"Interval Flip Bit", new KnapsackIntervalFlipBitMovementGenerator(&evl, n)},
            {"Interval Inversion", new KnapsackInversionMovementGenerator(&evl, n)}
        };

        for (auto &cm : cms) {
            KnapsackSolution s = cm.second(&evl);
            print_solution("Constructive Method: " + cm.first, &evl, &s);
            std::cout << std::endl;

            KnapsackSolution s1(0);
            for (auto &mg : mgs) {
                RHFirstImprovement<KnapsackSolution> fi(&evl, mg.second);
                LSHillClimbing<KnapsackSolution> hill_climbing_fi(&evl, &fi);
                s1 = hill_climbing_fi.run(s, 10);
                print_solution("Local Search: " + cm.first + " + Hill Climbing + First Improvement (" + mg.first + ")", &evl, &s1, &s);
                std::cout << std::endl;

                RHBestImprovement<KnapsackSolution> bi(&evl, mg.second);
                LSHillClimbing<KnapsackSolution> hill_climbing_bi(&evl, &bi);
                s1 = hill_climbing_bi.run(s, 10);
                print_solution("Local Search: " + cm.first + " + Hill Climbing + Best Improvement (" + mg.first + ")", &evl, &s1, &s);
                std::cout << std::endl;

                RHRandomSelection<KnapsackSolution> rs(&evl, mg.second, 5);
                RandomDescentMethod<KnapsackSolution> random_descent(&evl, &rs, 10);
                s1 = random_descent.run(s, 10);
                print_solution("Local Search: " + cm.first + " + Random Descent + Random Selection (" + mg.first + ")", &evl, &s1, &s);
                std::cout << std::endl;

                MHSimulatedAnnealing<KnapsackSolution> simulated_annealing(
                    &evl, mg.second, 100, 10,
                    0.99, 100000, 0.00001
                );
                s1 = simulated_annealing.run(s);
                print_solution("Meta Heuristic: " + cm.first + " + Simulated Annealing (" + mg.first + ")", &evl, &s1, &s);
                std::cout << std::endl;
            }

            std::cout << std::setw(100) << std::setfill('-') << "" << std::endl;
        }

        for (auto &mg : mgs)
            delete mg.second;
    }
}
