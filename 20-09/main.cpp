#include <iostream>
#include <vector>
#include "optimization.h"
#include "knapsack.h"
#include "neighborhood_exploration.h"


int main() {
    freopen("knapsack-input.txt", "r", stdin);
    freopen("knapsack-output.txt", "w", stdout);

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
    KnapsackSolution s = cm_knapsack_greedy_randomized(evl, 10, 0.1);
    Knapsack1FlipBitMovementGenerator mg(n);

    NEFindBest<KnapsackSolution> find_best(evl, mg);
    LSHillClimbing<KnapsackSolution> hill_climbing(evl, find_best);

    KnapsackSolution s1 = hill_climbing.run(s);
    std::cout << evl.evaluate(s1) << std::endl;
    for (int i=0; i<n; i++) {
        std::cout << s1.get(i) << " ";
    }
    std::cout << std::endl;


    NEFindAny<KnapsackSolution> find_any(evl, mg, 5);
    RandomDescentMethod<KnapsackSolution> random_descent(evl, find_any, 10);
    KnapsackSolution s2 = random_descent.run(s);
    std::cout << evl.evaluate(s2) << std::endl;
    for (int i=0; i<n; i++) {
        std::cout << s2.get(i) << " ";
    }
    std::cout << std::endl;
}
