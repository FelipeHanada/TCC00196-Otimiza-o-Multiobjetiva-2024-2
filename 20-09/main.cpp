#include <iostream>
#include <vector>
#include <iomanip>
#include "optimization.h"
#include "knapsack.h"
#include "neighborhood_exploration.h"

void print_solution(std::string label, KnapsackSolution &s) {
    std::cout << label << std::endl;
    std::cout << "Evaluation: " << s.get_evaluation() << " / ";
    for (int i=0; i<s.n; i++) {
        if (s.get(i))
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

        std::vector<std::pair<std::string, KnapsackSolution (*)(KnapsackEvaluator &evl)>> cms = {
            std::make_pair("Greedy", [](KnapsackEvaluator &evl) -> KnapsackSolution { return cm_knapsack_greedy(evl); }),
            std::make_pair("Random", [](KnapsackEvaluator &evl) -> KnapsackSolution { return cm_knapsack_random(evl, 10); }),
            std::make_pair("Greedy Randomized", [](KnapsackEvaluator &evl) -> KnapsackSolution { return cm_knapsack_greedy_randomized(evl, 10, 0.1); })
        };

        std::vector<std::pair<std::string, MovementGenerator<KnapsackSolution>*>> mgs = {
            std::make_pair("2 Flip Bit", new Knapsack2FlipBitMovementGenerator(evl, n)),
            std::make_pair("Interval Flip Bit", new KnapsackIntervalFlipBitMovementGenerator(evl, n)),
            std::make_pair("Interval Inversion", new KnapsackInversionMovementGenerator(evl, n))
        };

        for (auto &cm : cms) {
            KnapsackSolution s = cm.second(evl);
            print_solution("Constructive Method: " + cm.first, s);
            std::cout << std::endl;

            for (auto &mg : mgs) {
                RHFirstImprovement<KnapsackSolution> fi(evl, *mg.second);
                LSHillClimbing<KnapsackSolution> hill_climbing_fi(evl, fi);
                KnapsackSolution s1 = hill_climbing_fi.run(s, 10);
                print_solution("Local Search: " + cm.first + " + Hill Climbing + First Improvement (" + mg.first + ")", s1);
                std::cout << std::endl;

                RHBestImprovement<KnapsackSolution> bi(evl, *mg.second);
                LSHillClimbing<KnapsackSolution> hill_climbing_bi(evl, bi);
                KnapsackSolution s2 = hill_climbing_bi.run(s, 10);
                print_solution("Local Search: " + cm.first + " + Hill Climbing + Best Improvement (" + mg.first + ")", s2);
                std::cout << std::endl;

                RHRandomSelection<KnapsackSolution> rs(evl, *mg.second, 5);
                RandomDescentMethod<KnapsackSolution> random_descent(evl, rs, 10);
                KnapsackSolution s3 = random_descent.run(s, 10);
                print_solution("Local Search: " + cm.first + " + Random Descent + Random Selection (" + mg.first + ")", s3);
                std::cout << std::endl;
            }

            std::cout << std::setw(100) << std::setfill('-') << "" << std::endl;
        }
    }
}
