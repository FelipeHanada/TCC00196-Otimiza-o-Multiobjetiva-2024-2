#include "neighborhood_exploration.h"
#include <algorithm>
#include <cstdlib>

template <typename T, typename S>
NeighborhoodExplorationMethod<T, S>::NeighborhoodExplorationMethod(Evaluator<T, S> &evl, MovementGenerator<T> &mg)
    : evl(evl), mg(mg) {}

template <typename T, typename S>
NEFindAny<T, S>::NEFindAny(Evaluator<T, S> &evl, MovementGenerator<T> &mg, int k)
    : NeighborhoodExplorationMethod<T, S>(evl, mg), k(k) {}

template <typename T, typename S>
T NEFindAny<T, S>::run(const T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    T curr(s);
    int curr_value = this->evl.evaluate(s);
    for (int i = 0; i < this->k && movements.size() > 0; i++) {
        int m = rand() % movements.size();
        T s1 = movements[m]->move(s);
        int s1_value = this->evl.evaluate(s1);
        if (s1_value > curr_value) {
            curr = s1;
            curr_value = s1_value;
        }
        movements.erase(
            std::remove(movements.begin(), movements.end(), movements[m]),
            movements.end()
        );
    }

    return curr;
}

template <typename T, typename S>
NEFindFirst<T, S>::NEFindFirst(Evaluator<T, S> &evl, MovementGenerator<T> &mg)
    : NeighborhoodExplorationMethod<T, S>(evl, mg) {}

template <typename T, typename S>
T NEFindFirst<T, S>::run(const T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    int value = this->evl.evaluate(s);
    for (Movement<T> *m : movements) {
        T s1 = m->move(s);
        int s1_value = this->evl.evaluate(s1);
        if (s1_value > value) return s1;
    }
    return s;
}

template <typename T, typename S>
NEFindNext<T, S>::NEFindNext(Evaluator<T, S> &evl, MovementGenerator<T> &mg, int j)
    : NeighborhoodExplorationMethod<T, S>(evl, mg), j(j) {}

template <typename T, typename S>
T NEFindNext<T, S>::run(const T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    int value = this->evl.evaluate(s);
    for (int i = j + 1; i < movements.size(); i++) {
        T s1 = movements[i]->move(s);
        int s1_value = this->evl.evaluate(s1);
        if (s1_value > value) return s1;
    }
    return s;
}

template <typename T, typename S>
NEFindBest<T, S>::NEFindBest(Evaluator<T, S> &evl, MovementGenerator<T> &mg)
    : NeighborhoodExplorationMethod<T, S>(evl, mg) {}

template <typename T, typename S>
T NEFindBest<T, S>::run(const T &s) {
    std::vector<Movement<T>*> movements = this->mg.generate(s);

    T curr(s);
    int curr_value = this->evl.evaluate(s);
    for (Movement<T> *m : movements) {
        T s1 = m->move(s);
        int s1_value = this->evl.evaluate(s1);
        if (s1_value > curr_value) {
            curr = s1;
            curr_value = s1_value;
        }
    }
    return curr;
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
LocalSearch<T, S>::LocalSearch(Evaluator<T, S> &evl, NeighborhoodExplorationMethod<T, S> &ne)
    : evl(evl), ne(ne) {}

template <typename T, typename S>
LSHillClimbing<T, S>::LSHillClimbing(Evaluator<T, S> &evl, NeighborhoodExplorationMethod<T, S> &ne)
    : LocalSearch<T, S>(evl, ne) {}

template <typename T, typename S>
T LSHillClimbing<T, S>::run(const T &s) {
    T curr = s;
    int curr_value = this->evl.evaluate(s);
    while (true) {
        T s1 = this->ne.run(curr);
        int s1_value = this->evl.evaluate(s1);
        if (s1_value <= curr_value) break;
        curr = s1;
        curr_value = s1_value;
    }
    return curr;
}

template <typename T, typename S>
RandomDescentMethod<T, S>::RandomDescentMethod(Evaluator<T, S> &evl, NeighborhoodExplorationMethod<T, S> &ne, int k)
    : LocalSearch<T, S>(evl, ne), k(k) {}

template <typename T, typename S>
T RandomDescentMethod<T, S>::run(const T &s) {
    T curr = s;
    long long curr_value = this->evl.evaluate(s);
    int curr_k = k;
    while (curr_k > 0) {
        T s1 = this->ne.run(curr);
        int s1_value = this->evl.evaluate(s1);
        if (s1_value > curr_value) {
            curr = s1;
            curr_value = s1_value;
            curr_k = k;            
        } else {
            curr_k--;
        }
    }
    return curr;
}
