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
public:
    int n;
    bool *s; 
    KnapsackSolution(int n);
    KnapsackSolution(const KnapsackSolution &s);
    bool get(int i) const;
    void set(int i, bool x);
    void flip(int i);
};

class Knapsack1FlipBitMovement : public Movement<KnapsackSolution> {
    int i;
public:
    Knapsack1FlipBitMovement(int i);
    KnapsackSolution move(const KnapsackSolution &s) override;
};

class Knapsack1FlipBitMovementGenerator : public MovementGenerator<KnapsackSolution> {
    int n;
public:
    Knapsack1FlipBitMovementGenerator(int n);
    std::vector<Movement<KnapsackSolution>*> generate(const KnapsackSolution &s) override;
};

class KnapsackEvaluator : public Evaluator<KnapsackSolution, long long> {
public:
    int n;  // item quantity
    long long q;  // capacity
    std::vector<int> v;  // item values
    std::vector<int> w;  // item weights
    KnapsackEvaluator(int n, long long q, std::vector<int> v, std::vector<int> w);
    long long evaluate(const KnapsackSolution &s) override;
};

KnapsackSolution cm_knapsack_greedy_randomized(KnapsackEvaluator evl, float t, float a);

#endif // KNAPSACK_H
