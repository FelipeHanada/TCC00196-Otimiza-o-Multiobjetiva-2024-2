#ifndef KNAPSACK_H
#define KNAPSACK_H

#include "optimization.hpp"
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <numeric>
#include <climits>

class KnapsackEvaluator;

class KnapsackSolution : public Solution {
private:
    size_t n;
    bool *s;
public:
    mutable long long w;
    KnapsackSolution(int n);
    Solution* clone() const override;
    size_t size() const;
    bool get(int i) const;
    void set(int i, bool x, KnapsackEvaluator *evl = nullptr);
    void flip(int i, KnapsackEvaluator *evl = nullptr);
};

class KnapsackEvaluator : public Evaluator<KnapsackSolution> {
public:
    static const long long PUNISHMENT = LLONG_MIN;
    int n;  // item quantity
    long long q;  // capacity
    std::vector<int> v;  // item values
    std::vector<int> w;  // item weights
    KnapsackEvaluator(int n, long long q, std::vector<int> v, std::vector<int> w);
    long long evaluate(const KnapsackSolution *s) const override;
    long long get_evaluation(const KnapsackSolution *s) const override;
};

class KnapsackMovement : public Movement<KnapsackSolution> {
public:
    KnapsackEvaluator *evl;
    KnapsackMovement(KnapsackEvaluator *evl);
};

class Knapsack2FlipBitMovement : public KnapsackMovement {
private:
    int i, j;
public:
    Knapsack2FlipBitMovement(KnapsackEvaluator *evl, int i, int j);
    void move(KnapsackSolution *s) override;
    long long delta(const KnapsackSolution *s) const override;
};

class KnapsackIntervalFlipBitMovement : public KnapsackMovement {
private:
    int i, j;
public:
    KnapsackIntervalFlipBitMovement(KnapsackEvaluator *evl, int i, int j);
    void move(KnapsackSolution *s) override;
    long long delta(const KnapsackSolution *s) const override;
};

class KnapsackInversionMovement : public KnapsackMovement {
private:
    int i, j;
public:
    KnapsackInversionMovement(KnapsackEvaluator *evl, int i, int j);
    void move(KnapsackSolution *s) override;
    long long delta(const KnapsackSolution *s) const override;
};

class KnapsackMovementGenerator : public MovementGenerator<KnapsackSolution> {
public:
    KnapsackEvaluator *evl;
    KnapsackMovementGenerator(KnapsackEvaluator *evl);
};

class Knapsack2FlipBitMovementGenerator : public KnapsackMovementGenerator {
private:
    int n;
    int curr_i, curr_j;
public:
    Knapsack2FlipBitMovementGenerator(KnapsackEvaluator *evl, int n);
    std::vector<Movement<KnapsackSolution>*> get_all(const KnapsackSolution *s) override;
    Movement<KnapsackSolution>* get_random() override;
    bool has_next() override;
    Movement<KnapsackSolution>* next() override;
    void reset() override;
};

class KnapsackIntervalFlipBitMovementGenerator : public KnapsackMovementGenerator {
private:
    int n;
    int curr_i, curr_j;
public:
    KnapsackIntervalFlipBitMovementGenerator(KnapsackEvaluator *evl, int n);
    std::vector<Movement<KnapsackSolution>*> get_all(const KnapsackSolution *s) override;
    Movement<KnapsackSolution>* get_random() override;
    bool has_next() override;
    Movement<KnapsackSolution>* next() override;
    void reset() override;
};

class KnapsackInversionMovementGenerator : public KnapsackMovementGenerator {
private:
    int n;
    int curr_i, curr_j;
public:
    KnapsackInversionMovementGenerator(KnapsackEvaluator *evl, int n);
    std::vector<Movement<KnapsackSolution>*> get_all(const KnapsackSolution *s) override;
    Movement<KnapsackSolution>* get_random() override;
    bool has_next() override;
    Movement<KnapsackSolution>* next() override;
    void reset() override;
};

KnapsackSolution* cm_knapsack_greedy(const KnapsackEvaluator *evl);

KnapsackSolution* cm_knapsack_random(const KnapsackEvaluator *evl, float t);

KnapsackSolution* cm_knapsack_greedy_randomized(const KnapsackEvaluator *evl, float t, float a);

#endif // KNAPSACK_H
