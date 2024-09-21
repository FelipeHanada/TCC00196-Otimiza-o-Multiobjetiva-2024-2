#include <iostream>
#include <vector>
#include "optimization.h"
#include "knapsack.h"
#include "neighborhood_exploration.h"


int main() {
    freopen("knapsack-input.txt", "r", stdin);
    freopen("knapsack-output.txt", "w", stdout);

    int t;
    std::cin >> t;
    for (int test=0; test<t; test++) {
        std::cout << "[ ---------- TEST " << test << " ---------- ]" << std::endl;
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
        Knapsack1FlipBitMovementGenerator mg(n);
        KnapsackSolution s = cm_knapsack_greedy_randomized(evl, 999999999, 0.1);
        std::cout << "Constructive Method: Greedy Randomized" << std::endl;
        std::cout << evl.evaluate(s) << std::endl;
        for (int i=0; i<n; i++) {
            if (s.get(i))
                std::cout << i << " ";
        }
        std::cout << std::endl;

        NEFindBest<KnapsackSolution, long long> find_best(evl, mg);
        LSHillClimbing<KnapsackSolution, long long> hill_climbing(evl, find_best);
        KnapsackSolution s1 = hill_climbing.run(s);
        std::cout << "Local Search: Hill Climbing" << std::endl;
        std::cout << evl.evaluate(s1) << std::endl;
        for (int i=0; i<n; i++) {
            if (s1.get(i))
                std::cout << i << " ";
        }
        std::cout << std::endl;

        NEFindAny<KnapsackSolution, long long> find_any(evl, mg, 5);
        RandomDescentMethod<KnapsackSolution, long long> random_descent(evl, find_any, 10);
        KnapsackSolution s2 = random_descent.run(s);
        std::cout << "Local Search: Random Descent" << std::endl;
        std::cout << evl.evaluate(s2) << std::endl;
        for (int i=0; i<n; i++) {
            if (s2.get(i))
                std::cout << i << " ";
        }
        std::cout << std::endl;
    }
}
