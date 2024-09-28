#include "neighborhood_exploration.h"
#include <algorithm>
#include <cstdlib>
#include <optional>
#include <functional>

template <typename T, typename S>
NeighborhoodExplorationMethod<T, S>::NeighborhoodExplorationMethod(Evaluator<T, S> &evl, MovementGenerator<T> &mg)
    : evl(evl), mg(mg) {}

template <typename T, typename S>
NEFindAny<T, S>::NEFindAny(Evaluator<T, S> &evl, MovementGenerator<T> &mg, int k)
    : NeighborhoodExplorationMethod<T, S>(evl, mg), k(k) {}

template <typename T, typename S>
std::optional<std::reference_wrapper<Movement<T>>> NEFindAny<T, S>::get_movement(const T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    T curr(s);
    std::optional<std::reference_wrapper<Movement<T>>> curr_m = std::nullopt;
    int curr_value = this->evl.evaluate(s);
    for (int i = 0; i < this->k && movements.size() > 0; i++) {
        int r = std::rand() % movements.size();
        Movement<T> *m = movements[r];
        T s1 = m->move(s);
        int s1_value = this->evl.evaluate(s1);
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

template <typename T, typename S>
NEFindFirst<T, S>::NEFindFirst(Evaluator<T, S> &evl, MovementGenerator<T> &mg)
    : NeighborhoodExplorationMethod<T, S>(evl, mg) {}

template <typename T, typename S>
std::optional<std::reference_wrapper<Movement<T>>> NEFindFirst<T, S>::get_movement(const T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    int value = this->evl.evaluate(s);
    for (Movement<T> *m : movements) {
        T s1 = m->move(s);
        int s1_value = this->evl.evaluate(s1);
        if (s1_value > value) {
            return *m;
        }
    }
    return std::nullopt;
}

template <typename T, typename S>
NEFindNext<T, S>::NEFindNext(Evaluator<T, S> &evl, MovementGenerator<T> &mg, int j)
    : NeighborhoodExplorationMethod<T, S>(evl, mg), j(j) {}

template <typename T, typename S>
std::optional<std::reference_wrapper<Movement<T>>> NEFindNext<T, S>::get_movement(const T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    int value = this->evl.evaluate(s);
    for (int i = j + 1; i < movements.size(); i++) {
        Movement<T> m = movements[i];
        T s1 = m->move(s);
        int s1_value = this->evl.evaluate(s1);
        if (s1_value > value) return m;
    }
    return std::nullopt;
}

template <typename T, typename S>
NEFindBest<T, S>::NEFindBest(Evaluator<T, S> &evl, MovementGenerator<T> &mg)
    : NeighborhoodExplorationMethod<T, S>(evl, mg) {}

template <typename T, typename S>
std::optional<std::reference_wrapper<Movement<T>>> NEFindBest<T, S>::get_movement(const T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    T curr(s);
    std::optional<std::reference_wrapper<Movement<T>>> curr_m = std::nullopt;
    int curr_value = this->evl.evaluate(s);
    for (Movement<T> *m : movements) {
        T s1 = m->move(s);
        int s1_value = this->evl.evaluate(s1);
        if (s1_value > curr_value) {
            curr = s1;
            curr_m = *m;
            curr_value = s1_value;
        }
    }

    return curr_m;
}

template <typename T, typename S>
std::vector<T> all_neighbors(T &s, MovementGenerator<T> &mg) {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");

    std::vector<T> r;
    for (Movement<T> *m : mg.get(s)) {
        r.push_back(m->move(s));
    }
    return r;
}

template <typename T, typename S>
RefinementHeuristicsMethod<T, S>::RefinementHeuristicsMethod(Evaluator<T, S> &evl, MovementGenerator<T> &mg)
    : evl(evl), mg(mg) {}

template <typename T, typename S>
RHRandomSelection<T, S>::RHRandomSelection(Evaluator<T, S> &evl, MovementGenerator<T> &mg, int k)
    : RefinementHeuristicsMethod<T, S>(evl, mg), k(k) {}

template <typename T, typename S>
std::optional<T> RHRandomSelection<T, S>::run(const T &s) {
    NEFindAny<T, S> ne(this->evl, this->mg, this->k);
    std::optional<std::reference_wrapper<Movement<T>>> m = ne.get_movement(s);

    if (m.has_value()) {
        T s1 = m.value().get().move(s);
        return s1;
    }

    return std::nullopt;
}

template <typename T, typename S>
RHFirstImprovement<T, S>::RHFirstImprovement(Evaluator<T, S> &evl, MovementGenerator<T> &mg)
    : RefinementHeuristicsMethod<T, S>(evl, mg) {}

template <typename T, typename S>
std::optional<T> RHFirstImprovement<T, S>::run(const T &s) {
    NEFindFirst<T, S> ne(this->evl, this->mg);
    std::optional<std::reference_wrapper<Movement<T>>> m = ne.get_movement(s);

    if (m.has_value()) {
        T s1 = m.value().get().move(s);
        return s1;
    }

    return std::nullopt;
}

template <typename T, typename S>
RHBestImprovement<T, S>::RHBestImprovement(Evaluator<T, S> &evl, MovementGenerator<T> &mg)
    : RefinementHeuristicsMethod<T, S>(evl, mg) {}

template <typename T, typename S>
std::optional<T> RHBestImprovement<T, S>::run(const T &s) {
    NEFindBest<T, S> ne(this->evl, this->mg);
    std::optional<std::reference_wrapper<Movement<T>>> m = ne.get_movement(s);

    if (m.has_value()) {
        T s1 = m.value().get().move(s);
        return s1;
    }

    return std::nullopt;
}

template <typename T, typename S>
LocalSearch<T, S>::LocalSearch(Evaluator<T, S> &evl, RefinementHeuristicsMethod<T, S> &rh)
    : evl(evl), rh(rh) {}

template <typename T, typename S>
LSHillClimbing<T, S>::LSHillClimbing(Evaluator<T, S> &evl, RefinementHeuristicsMethod<T, S> &rh)
    : LocalSearch<T, S>(evl, rh) {}

template <typename T, typename S>
T LSHillClimbing<T, S>::run(const T &s) {
    T curr = s;
    int curr_value = this->evl.evaluate(s);
    while (true) {
        std::optional<T> s1 = this->rh.run(curr);
        if (!s1.has_value()) break;
        
        int s1_value = this->evl.evaluate(s1.value());
        if (s1_value <= curr_value) break;
        curr = s1.value();
        curr_value = s1_value;
    }
    return curr;
}

template <typename T, typename S>
RandomDescentMethod<T, S>::RandomDescentMethod(Evaluator<T, S> &evl, RefinementHeuristicsMethod<T, S> &rh, int k)
    : LocalSearch<T, S>(evl, rh), k(k) {}

template <typename T, typename S>
T RandomDescentMethod<T, S>::run(const T &s) {
    T curr = s;
    long long curr_value = this->evl.evaluate(s);
    int curr_k = k;
    while (curr_k > 0) {
        std::optional<T> s1 = this->rh.run(curr);
        if (!s1.has_value()) break;

        int s1_value = this->evl.evaluate(s1.value());
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
