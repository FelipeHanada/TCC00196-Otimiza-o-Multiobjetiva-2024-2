#include "neighborhood_exploration.h"

template <typename T>
NeighborhoodExplorationMethod<T>::NeighborhoodExplorationMethod(Evaluator<T> &evl, MovementGenerator<T> &mg)
    : evl(evl), mg(mg) {}

template <typename T>
NEFindAny<T>::NEFindAny(Evaluator<T> &evl, MovementGenerator<T> &mg, int k)
    : NeighborhoodExplorationMethod<T>(evl, mg), k(k) {}

template <typename T>
std::optional<std::reference_wrapper<Movement<T>>> NEFindAny<T>::get_movement(T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    T curr(s);
    std::optional<std::reference_wrapper<Movement<T>>> curr_m = std::nullopt;
    long long curr_value = this->evl.get_evaluation(s);
    for (int i = 0; i < this->k && movements.size() > 0; i++) {
        int r = std::rand() % movements.size();
        Movement<T> *m = movements[r];
        T s1 = m->move(s);
        long long s1_value = this->evl.get_evaluation(s1);
        if (s1_value > curr_value) {
            curr = s1;
            curr_m = *m;
            curr_value = s1_value;
        }
        movements.erase(
            std::remove(movements.begin(), movements.end(), movements[r]),
            movements.end()
        );
    }

    return curr_m;
}

template <typename T>
NEFindFirst<T>::NEFindFirst(Evaluator<T> &evl, MovementGenerator<T> &mg)
    : NeighborhoodExplorationMethod<T>(evl, mg) {}

template <typename T>
std::optional<std::reference_wrapper<Movement<T>>> NEFindFirst<T>::get_movement(T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    long long value = this->evl.get_evaluation(s);
    for (Movement<T> *m : movements) {
        T s1 = m->move(s);
        long long s1_value = this->evl.get_evaluation(s1);
        if (s1_value > value) {
            return *m;
        }
    }
    return std::nullopt;
}

template <typename T>
NEFindNext<T>::NEFindNext(Evaluator<T> &evl, MovementGenerator<T> &mg, int j)
    : NeighborhoodExplorationMethod<T>(evl, mg), j(j) {}

template <typename T>
std::optional<std::reference_wrapper<Movement<T>>> NEFindNext<T>::get_movement(T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    long long value = this->evl.get_evaluation(s);
    for (int i = j + 1; i < movements.size(); i++) {
        Movement<T> m = movements[i];
        T s1 = m->move(s);
        long long s1_value = this->evl.get_evaluation(s1);
        if (s1_value > value) return m;
    }
    return std::nullopt;
}

template <typename T>
NEFindBest<T>::NEFindBest(Evaluator<T> &evl, MovementGenerator<T> &mg)
    : NeighborhoodExplorationMethod<T>(evl, mg) {}

template <typename T>
std::optional<std::reference_wrapper<Movement<T>>> NEFindBest<T>::get_movement(T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    T curr(s);
    std::optional<std::reference_wrapper<Movement<T>>> curr_m = std::nullopt;
    long long curr_value = this->evl.get_evaluation(s);
    for (Movement<T> *m : movements) {
        T s1 = m->move(s);
        long long s1_value = this->evl.get_evaluation(s1);
        if (s1_value > curr_value) {
            curr = s1;
            curr_m = *m;
            curr_value = s1_value;
        }
    }

    return curr_m;
}

template <typename T>
std::vector<T> all_neighbors(T &s, MovementGenerator<T> &mg) {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");

    std::vector<T> r;
    for (Movement<T> *m : mg.get(s)) {
        r.push_back(m->move(s));
    }
    return r;
}

template <typename T>
RefinementHeuristicsMethod<T>::RefinementHeuristicsMethod(Evaluator<T> &evl, MovementGenerator<T> &mg)
    : evl(evl), mg(mg) {}

template <typename T>
RHRandomSelection<T>::RHRandomSelection(Evaluator<T> &evl, MovementGenerator<T> &mg, int k)
    : RefinementHeuristicsMethod<T>(evl, mg), k(k) {}

template <typename T>
std::optional<T> RHRandomSelection<T>::run(T &s) {
    NEFindAny<T> ne(this->evl, this->mg, this->k);
    std::optional<std::reference_wrapper<Movement<T>>> m = ne.get_movement(s);

    if (m.has_value()) {
        Movement<T> &m1 = m.value().get();
        T s1 = m1.move(s);
        return s1;
    }

    return std::nullopt;
}

template <typename T>
RHFirstImprovement<T>::RHFirstImprovement(Evaluator<T> &evl, MovementGenerator<T> &mg)
    : RefinementHeuristicsMethod<T>(evl, mg) {}

template <typename T>
std::optional<T> RHFirstImprovement<T>::run(T &s) {
    NEFindFirst<T> ne(this->evl, this->mg);
    std::optional<std::reference_wrapper<Movement<T>>> m = ne.get_movement(s);

    if (m.has_value()) {
        Movement<T> &m1 = m.value().get();
        T s1 = m1.move(s);
        return s1;
    }

    return std::nullopt;
}

template <typename T>
RHBestImprovement<T>::RHBestImprovement(Evaluator<T> &evl, MovementGenerator<T> &mg)
    : RefinementHeuristicsMethod<T>(evl, mg) {}

template <typename T>
std::optional<T> RHBestImprovement<T>::run(T &s) {
    NEFindBest<T> ne(this->evl, this->mg);
    std::optional<std::reference_wrapper<Movement<T>>> m = ne.get_movement(s);

    if (m.has_value()) {
        Movement<T> &m1 = m.value().get();
        T s1 = m1.move(s);
        return s1;
    }

    return std::nullopt;
}

template <typename T>
LocalSearch<T>::LocalSearch(Evaluator<T> &evl, RefinementHeuristicsMethod<T> &rh)
    : evl(evl), rh(rh) {}

template <typename T>
LSHillClimbing<T>::LSHillClimbing(Evaluator<T> &evl, RefinementHeuristicsMethod<T> &rh)
    : LocalSearch<T>(evl, rh) {}

template <typename T>
T LSHillClimbing<T>::run(T &s, float t) {
    auto start = std::chrono::high_resolution_clock::now();

    T curr = s;
    long long curr_value = this->evl.get_evaluation(s);
    while (true) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<float>(current - start).count() > t)
            break;

        std::optional<T> s1 = this->rh.run(curr);
        if (!s1.has_value()) break;
        
        long long s1_value = this->evl.get_evaluation(s1.value());
        if (s1_value <= curr_value) break;
        curr = s1.value();
        curr_value = s1_value;
    }
    return curr;
}

template <typename T>
RandomDescentMethod<T>::RandomDescentMethod(Evaluator<T> &evl, RefinementHeuristicsMethod<T> &rh, int k)
    : LocalSearch<T>(evl, rh), k(k) {}

template <typename T>
T RandomDescentMethod<T>::run(T &s, float t) {
    auto start = std::chrono::high_resolution_clock::now();

    T curr = s;
    long long curr_value = this->evl.get_evaluation(s);
    int curr_k = this->k;
    while (curr_k > 0) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<float>(current - start).count() > t)
            break;

        std::optional<T> s1 = this->rh.run(curr);
        if (!s1.has_value()) break;

        long long s1_value = this->evl.get_evaluation(s1.value());
        if (s1_value > curr_value) {
            curr = s1.value();
            curr_value = s1_value;
            curr_k = k;            
        } else {
            curr_k--;
        }
    }
    return curr;
}
