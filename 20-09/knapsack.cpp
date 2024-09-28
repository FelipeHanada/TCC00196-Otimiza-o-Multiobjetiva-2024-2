#include "knapsack.h"


KnapsackSolution::KnapsackSolution(int n)
    : Solution()
{
    this->n = n;
    this->s = new bool[n];
    for (int i = 0; i < n; i++)
        this->s[i] = false;
}

KnapsackSolution::KnapsackSolution(const KnapsackSolution &s)
    : KnapsackSolution(s.n)
{
    for (int i = 0; i < s.n; i++)
        this->s[i] = s.s[i];
}

bool KnapsackSolution::get(int i) const {
    return s[i];
}

void KnapsackSolution::set(int i, bool x) {
    s[i] = x;
    this->set_evaluated(false);
}

void KnapsackSolution::flip(int i) {
    s[i] = !s[i];
    this->set_evaluated(false);
}

KnapsackEvaluator::KnapsackEvaluator(int n, long long q, std::vector<int> v, std::vector<int> w) {
    this->n = n;
    this->q = q;
    this->v = v;
    this->w = w;
}

long long KnapsackEvaluator::evaluate(const KnapsackSolution &s) {
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

KnapsackMovement::KnapsackMovement(KnapsackEvaluator &evl) : evl(evl) {}

KnapsackMovementGenerator::KnapsackMovementGenerator(KnapsackEvaluator &evl) : evl(evl) {}

Knapsack2FlipBitMovement::Knapsack2FlipBitMovement(KnapsackEvaluator &evl, int i, int j)
    : KnapsackMovement(evl)
{
    this->i = i;
    this->j = j;
}

KnapsackSolution Knapsack2FlipBitMovement::move(const KnapsackSolution &s) {
    KnapsackSolution s1(s);
    s1.flip(this->i);
    s1.flip(this->j);
    s1.set_evaluation(s1.get_evaluation() + (s1.get(this->i) ? 1 : -1) * this->evl.v[this->i]);
    s1.set_evaluation(s1.get_evaluation() + (s1.get(this->j) ? 1 : -1) * this->evl.v[this->j]);
    return s1;
}

KnapsackIntervalFlipBitMovement::KnapsackIntervalFlipBitMovement(KnapsackEvaluator &evl, int i, int j)
    : KnapsackMovement(evl)
{
    this->i = i;
    this->j = j;
}

KnapsackSolution KnapsackIntervalFlipBitMovement::move(const KnapsackSolution &s) {
    KnapsackSolution s1(s);
    for (int k = this->i; k <= this->j; k++) {
        s1.flip(k);
        s1.set_evaluation(s1.get_evaluation() + (s1.get(k) ? 1 : -1) * this->evl.v[k]);
    }

    return s1;
}

KnapsackInversionMovement::KnapsackInversionMovement(KnapsackEvaluator &evl, int i, int j)
    : KnapsackMovement(evl)
{
    this->i = i;
    this->j = j;
}

KnapsackSolution KnapsackInversionMovement::move(const KnapsackSolution &s) {
    KnapsackSolution s1(s);
    for (int k = 0; k < (this->j - this->i + 1) / 2; k++) {
        int a = this->i + k, b = this->j - k;
        s1.flip(a);
        s1.set_evaluation(s1.get_evaluation() + (s1.get(a) ? 1 : -1) * this->evl.v[a]);
        s1.flip(b);
        s1.set_evaluation(s1.get_evaluation() + (s1.get(b) ? 1 : -1) * this->evl.v[b]);
    }
    return s1;
}

Knapsack2FlipBitMovementGenerator::Knapsack2FlipBitMovementGenerator(KnapsackEvaluator &evl, int n)
    : KnapsackMovementGenerator(evl)
{
    this->n = n;
}

std::vector<Movement<KnapsackSolution>*> Knapsack2FlipBitMovementGenerator::generate(const KnapsackSolution &s) {
    std::vector<Movement<KnapsackSolution>*> movements;
    for (int i = 0; i < this->n; i++) {
        for (int j = i + 1; j < this->n; j++) {
            movements.push_back(new Knapsack2FlipBitMovement(this->evl, i, j));
        }
    }
    return movements;
}

KnapsackIntervalFlipBitMovementGenerator::KnapsackIntervalFlipBitMovementGenerator(KnapsackEvaluator &evl, int n)
    : KnapsackMovementGenerator(evl)
{
    this->n = n;
}

std::vector<Movement<KnapsackSolution>*> KnapsackIntervalFlipBitMovementGenerator::generate(const KnapsackSolution &s) {
    std::vector<Movement<KnapsackSolution>*> movements;
    for (int i = 0; i < this->n; i++) {
        for (int j = i; j < this->n; j++) {
            movements.push_back(new KnapsackIntervalFlipBitMovement(this->evl, i, j));
        }
    }
    return movements;
}

KnapsackInversionMovementGenerator::KnapsackInversionMovementGenerator(KnapsackEvaluator &evl, int n)
    : KnapsackMovementGenerator(evl)
{
    this->n = n;
}

std::vector<Movement<KnapsackSolution>*> KnapsackInversionMovementGenerator::generate(const KnapsackSolution &s) {
    std::vector<Movement<KnapsackSolution>*> movements;
    for (int i = 0; i < this->n; i++) {
        for (int j = i; j < this->n; j++) {
            movements.push_back(new KnapsackInversionMovement(evl, i, j));
        }
    }
    return movements;
}

KnapsackSolution cm_knapsack_greedy(const KnapsackEvaluator &evl) {
    long long curr_Q = evl.q;
    KnapsackSolution s(evl.n);

    std::vector<int> c(evl.n);
    std::iota(c.begin(), c.end(), 0);
    std::sort(c.begin(), c.end(), [&](const int x, const int y) {
        float qx = evl.v[x] / (float) evl.w[x],
              qy = evl.v[y] / (float) evl.w[y];
        return qx > qy;
    });

    for (int g : c) {
        if (evl.w[g] > curr_Q) {
            continue;
        }

        curr_Q -= evl.w[g];
        s.set(g, true);
    }

    return s;
}

KnapsackSolution cm_knapsack_random(const KnapsackEvaluator &evl, float t) {
    auto start = std::chrono::high_resolution_clock::now();

    long long curr_Q = evl.q;
    KnapsackSolution s(evl.n);

    std::vector<int> c(evl.n);
    iota(c.begin(), c.end(), 0);
    sort(c.begin(), c.end(), [&](const int x, const int y) {
        return evl.w[x] < evl.w[y];
    });

    while (c.size() > 0 && curr_Q < evl.w[(*c.rbegin())]) {
        c.pop_back();
    }

    while (c.size() > 0 && curr_Q >= evl.w[c[0]]) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<float>(current - start).count() > t)
            break;

        auto g = c.begin() + (rand() % c.size());
        s.set(*g, true);
        curr_Q -= evl.w[*g];
        c.erase(g);

        while (c.size() > 0 && curr_Q < evl.w[(*c.rbegin())]) {
           c.pop_back();
        }
    }

    return s;
}

KnapsackSolution cm_knapsack_greedy_randomized(const KnapsackEvaluator &evl, float t, float a) {
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
