#include "knapsack.h"

KnapsackSolution::KnapsackSolution(int n) {
    this->s = new bool[n];
    for (int i = 0; i < n; i++)
        this->s[i] = false;
}

KnapsackSolution::KnapsackSolution(const KnapsackSolution &s)
    : KnapsackSolution(sizeof(s.s))
{
    for (int i = 0; i < sizeof(s.s); i++)
        this->s[i] = s.s[i];
}

bool KnapsackSolution::get(int i) {
    return s[i];
}

void KnapsackSolution::set(int i, bool x) {
    s[i] = x;
}

void KnapsackSolution::flip(int i) {
    s[i] = !s[i];
}

KnapsackEvaluator::KnapsackEvaluator(int n, long long q, std::vector<int> v, std::vector<int> w) {
    this->n = n;
    this->q = q;
    this->v = v;
    this->w = w;
}

long long KnapsackEvaluator::evaluate(KnapsackSolution &s) {
    long long curr_q = this->q;
    long long curr_value = 0;
    for (int i = 0; i < this->n && curr_q > 0; i++) {
        if (s.get(i)) {
            curr_value += this->v[i];
            curr_q -= this->w[i];
        }
    }
    if (curr_q < 0) return INT32_MIN;
    return curr_value;
}

Knapsack1FlipBitMovement::Knapsack1FlipBitMovement(int i) {
    this->i = i;
}

KnapsackSolution Knapsack1FlipBitMovement::move(const KnapsackSolution &s) {
    KnapsackSolution s1(s);
    s1.flip(this->i);
    return s1;
}

Knapsack1FlipBitMovementGenerator::Knapsack1FlipBitMovementGenerator(int n)
    : MovementGenerator<KnapsackSolution>()
{
    this->n = n;
}

std::vector<Movement<KnapsackSolution>*> Knapsack1FlipBitMovementGenerator::generate(KnapsackSolution &s) {
    std::vector<Movement<KnapsackSolution>*> movements;
    for (int i = 0; i < this->n; i++) {
        movements.push_back(new Knapsack1FlipBitMovement(i));
    }
    return movements;
}

KnapsackSolution cm_knapsack_greedy_randomized(KnapsackEvaluator evl, float t, float a) {
    auto start = std::chrono::high_resolution_clock::now();

    KnapsackSolution s(evl.n);
    long long curr_Q = evl.q;

    std::vector<float> values;
    for (int i = 0; i < evl.n; i++) {
        float value = evl.v[i] / (float) evl.w[i];
        values.push_back(value);
    }

    std::vector<int> c(evl.n), c_value_ordered(evl.n);
    std::iota(c.begin(), c.end(), 0);
    std::iota(c_value_ordered.begin(), c_value_ordered.end(), 0);

    std::sort(c.rbegin(), c.rend(), [&](const int x, const int y) {
        return evl.w[x] < evl.w[y];
    });

    std::sort(c_value_ordered.rbegin(), c_value_ordered.rend(), [&](const int x, const int y) {
        return values[x] < values[y];
    });

    while (c.size() > 0 && evl.w[*c.begin()] > curr_Q) {
        c_value_ordered.erase(
            std::remove(c_value_ordered.begin(), c_value_ordered.end(), *c.begin()),
            c_value_ordered.end()
        );
        c.erase(c.begin());
    }

    while (c.size() > 0) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<float>(current - start).count() > t)
            break;

        float min_value = values[c_value_ordered.back()],
              max_value = values[c_value_ordered.front()];

        float threshold = max_value - a * (max_value - min_value);

        auto rc_end = std::lower_bound(
            c_value_ordered.begin(),
            c_value_ordered.end(),
            threshold,
            [&](const int x, const float threshold) {
                return values[x] >= threshold;
            }
        );

        int g = c_value_ordered[std::rand() % (std::distance(c_value_ordered.begin(), rc_end))];

        curr_Q -= evl.w[g];
        s.set(g, true);

        c.erase(std::remove(c.begin(), c.end(), g), c.end());
        c_value_ordered.erase(std::remove(c_value_ordered.begin(), c_value_ordered.end(), g), c_value_ordered.end());

        while (c.size() > 0 && evl.w[*c.begin()] > curr_Q) {
            c_value_ordered.erase(
                remove(c_value_ordered.begin(), c_value_ordered.end(), *c.begin()),
                c_value_ordered.end()
            );
            c.erase(c.begin());
        }
    }

    return s;
}
