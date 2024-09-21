#ifndef KNAPSACK_H
#define KNAPSACK_H

#include "optimization.h"
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <numeric>
#include <climits>

class KnapsackSolution : public Solution {
private:
    int s = 0;
public:
    bool get(int i);
    void set(int i, bool x);
    void flip(int i);
};

class Knapsack1FlipBitMovement : public Movement<KnapsackSolution> {
    int i;
public:
    Knapsack1FlipBitMovement(int i);
    KnapsackSolution move(KnapsackSolution s) override;
};

class Knapsack1FlipBitMovementGenerator : public MovementGenerator<KnapsackSolution> {
    int n;
public:
    Knapsack1FlipBitMovementGenerator(int n);
    std::vector<Movement<KnapsackSolution>*> generate(KnapsackSolution s) override;
};

class KnapsackEvaluator : public Evaluator<KnapsackSolution> {
public:
    int n;  // item quantity
    int q;  // capacity
    std::vector<int> v;  // item values
    std::vector<int> w;  // item weights
    KnapsackEvaluator(int n, int q, std::vector<int> v, std::vector<int> w);
    int evaluate(KnapsackSolution s) override;
};

KnapsackSolution cm_knapsack_greedy_randomized(KnapsackEvaluator evl, float t, float a);

#endif // KNAPSACK_H
