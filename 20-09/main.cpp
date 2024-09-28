#include <iostream>
#include <vector>
#include <iomanip>
#include "optimization.h"
#include "knapsack.h"
#include "neighborhood_exploration.h"

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
            std::cout << "Constructive Method: " << cm.first << std::endl;
            std::cout << evl.evaluate(s) << std::endl;
            for (int i=0; i<n; i++) {
                if (s.get(i))
                    std::cout << i << " ";
            }
            std::cout << std::endl;

            for (auto &mg : mgs) {
                RHFirstImprovement<KnapsackSolution> fi(evl, *mg.second);
                LSHillClimbing<KnapsackSolution> hill_climbing_fi(evl, fi);
                KnapsackSolution s1 = hill_climbing_fi.run(s, 10);
                std::cout << "Local Search: " << cm.first << " + Hill Climbing + First Improvement (" << mg.first << ")" << std::endl;
                std::cout << evl.evaluate(s1) << std::endl;
                for (int i=0; i<n; i++) {
                    if (s1.get(i))
                        std::cout << i << " ";
                }
                std::cout << std::endl;

                RHBestImprovement<KnapsackSolution> bi(evl, *mg.second);
                LSHillClimbing<KnapsackSolution> hill_climbing_bi(evl, bi);
                KnapsackSolution s2 = hill_climbing_bi.run(s, 10);
                std::cout << "Local Search: " << cm.first << " + Hill Climbing + Best Improvement (" << mg.first << ")" << std::endl;
                std::cout << evl.evaluate(s2) << std::endl;
                for (int i=0; i<n; i++) {
                    if (s2.get(i))
                        std::cout << i << " ";
                }
                std::cout << std::endl;

                RHRandomSelection<KnapsackSolution> rs(evl, *mg.second, 5);
                RandomDescentMethod<KnapsackSolution> random_descent(evl, rs, 10);
                KnapsackSolution s3 = random_descent.run(s, 10);
                std::cout << "Local Search: " << cm.first << " + Random Descent + Random Selection (" << mg.first << ")" << std::endl;
                std::cout << evl.evaluate(s3) << std::endl;
                for (int i=0; i<n; i++) {
                    if (s3.get(i))
                        std::cout << i << " ";
                }
                std::cout << std::endl;
            }

            std::cout << std::setw(100) << std::setfill('-') << "" << std::endl;
        }
    }
}
