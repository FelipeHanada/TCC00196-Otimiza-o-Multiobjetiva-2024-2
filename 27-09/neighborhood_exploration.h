#ifndef NEIGHBORHOOD_EXPLORATION_H
#define NEIGHBORHOOD_EXPLORATION_H

#include <vector>
#include <optional>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include "optimization.h"

template <typename T>
class NeighborhoodExplorationMethod {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    Evaluator<T> &evl;
    MovementGenerator<T> &mg;
    NeighborhoodExplorationMethod(Evaluator<T> &evl, MovementGenerator<T> &mg);
    virtual std::optional<std::reference_wrapper<Movement<T>>> get_movement(T &s) = 0;
};

template <typename T>
class NEFindAny : public NeighborhoodExplorationMethod<T> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int k;
public:
    NEFindAny(Evaluator<T> &evl, MovementGenerator<T> &mg, int k);
    std::optional<std::reference_wrapper<Movement<T>>> get_movement(T &s) override;
};

template <typename T>
class NEFindFirst : public NeighborhoodExplorationMethod<T> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    NEFindFirst(Evaluator<T> &evl, MovementGenerator<T> &mg);
    std::optional<std::reference_wrapper<Movement<T>>> get_movement(T &s) override;
};

template <typename T>
class NEFindNext : public NeighborhoodExplorationMethod<T> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int j;
public:
    NEFindNext(Evaluator<T> &evl, MovementGenerator<T> &mg, int j);
    std::optional<std::reference_wrapper<Movement<T>>> get_movement(T &s) override;
};

template <typename T>
class NEFindBest : public NeighborhoodExplorationMethod<T> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    NEFindBest(Evaluator<T> &evl, MovementGenerator<T> &mg);
    std::optional<std::reference_wrapper<Movement<T>>> get_movement(T &s) override;
};

template <typename T>
std::vector<T> all_neighbors(T &s, MovementGenerator<T> &mg);

template <typename T>
class RefinementHeuristicsMethod {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    Evaluator<T> &evl;
    MovementGenerator<T> &mg;
    RefinementHeuristicsMethod(Evaluator<T> &evl, MovementGenerator<T> &mg);
    virtual std::optional<T> run(T &s) = 0;
};

template <typename T>
class RHRandomSelection : public RefinementHeuristicsMethod<T> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int k;
public:
    RHRandomSelection(Evaluator<T> &evl, MovementGenerator<T> &mg, int k);
    std::optional<T> run(T &s) override;
};

template <typename T>
class RHFirstImprovement : public RefinementHeuristicsMethod<T> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    RHFirstImprovement(Evaluator<T> &evl, MovementGenerator<T> &mg);
    std::optional<T> run(T &s) override;
};

template <typename T>
class RHBestImprovement : public RefinementHeuristicsMethod<T> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    RHBestImprovement(Evaluator<T> &evl, MovementGenerator<T> &mg);
    std::optional<T> run(T &s) override;
};

template <typename T>
class LocalSearch {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    Evaluator<T> &evl;
    RefinementHeuristicsMethod<T> &rh;
    LocalSearch(Evaluator<T> &evl, RefinementHeuristicsMethod<T> &rh);
    virtual T run(T &s, float t) = 0;
};

template <typename T>
class LSHillClimbing : public LocalSearch<T> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    LSHillClimbing(Evaluator<T> &evl, RefinementHeuristicsMethod<T> &rh);
    T run(T &s, float t) override;
};

template <typename T>
class RandomDescentMethod : public LocalSearch<T> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int k;
public:
    RandomDescentMethod(Evaluator<T> &evl, RefinementHeuristicsMethod<T> &rh, int k);
    T run(T &s, float t) override;
};

#include "neighborhood_exploration.tpp"

#endif // NEIGHBORHOOD_EXPLORATION_H
