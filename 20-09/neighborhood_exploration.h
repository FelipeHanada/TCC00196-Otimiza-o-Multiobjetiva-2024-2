#ifndef NEIGHBORHOOD_EXPLORATION_H
#define NEIGHBORHOOD_EXPLORATION_H

#include "optimization.h"
#include <vector>

template <typename T, typename S>
class NeighborhoodExplorationMethod {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    Evaluator<T, S> &evl;
    MovementGenerator<T> &mg;
    NeighborhoodExplorationMethod(Evaluator<T, S> &evl, MovementGenerator<T> &mg);
    virtual T run(T &s) = 0;
};

template <typename T, typename S>
class NEFindAny : public NeighborhoodExplorationMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int k;
public:
    NEFindAny(Evaluator<T, S> &evl, MovementGenerator<T> &mg, int k);
    T run(T &s) override;
};

template <typename T, typename S>
class NEFindFirst : public NeighborhoodExplorationMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    NEFindFirst(Evaluator<T, S> &evl, MovementGenerator<T> &mg);
    T run(T &s) override;
};

template <typename T, typename S>
class NEFindNext : public NeighborhoodExplorationMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int j;
public:
    NEFindNext(Evaluator<T, S> &evl, MovementGenerator<T> &mg, int j);
    T run(T &s) override;
};

template <typename T, typename S>
class NEFindBest : public NeighborhoodExplorationMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    NEFindBest(Evaluator<T, S> &evl, MovementGenerator<T> &mg);
    T run(T &s) override;
};

template <typename T>
std::vector<T> all_neighbors(T &s, MovementGenerator<T> &mg);

template <typename T, typename S>
class LocalSearch {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    Evaluator<T, S> &evl;
    NeighborhoodExplorationMethod<T, S> &ne;
    LocalSearch(Evaluator<T, S> &evl, NeighborhoodExplorationMethod<T, S> &ne);
    virtual T run(T &s) = 0;
};

template <typename T, typename S>
class LSHillClimbing : public LocalSearch<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    LSHillClimbing(Evaluator<T, S> &evl, NeighborhoodExplorationMethod<T, S> &ne);
    T run(T &s) override;
};

template <typename T, typename S>
class RandomDescentMethod : public LocalSearch<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int k;
public:
    RandomDescentMethod(Evaluator<T, S> &evl, NeighborhoodExplorationMethod<T, S> &ne, int k);
    T run(T &s) override;
};

#include "neighborhood_exploration.tpp"

#endif // NEIGHBORHOOD_EXPLORATION_H
