#ifndef NEIGHBORHOOD_EXPLORATION_H
#define NEIGHBORHOOD_EXPLORATION_H

#include "optimization.h"
#include <vector>
#include <optional>


template <typename T, typename S>
class NeighborhoodExplorationMethod {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    Evaluator<T, S> &evl;
    MovementGenerator<T> &mg;
    NeighborhoodExplorationMethod(Evaluator<T, S> &evl, MovementGenerator<T> &mg);
    virtual std::optional<std::reference_wrapper<Movement<T>>> get_movement(const T &s) = 0;
};

template <typename T, typename S>
class NEFindAny : public NeighborhoodExplorationMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int k;
public:
    NEFindAny(Evaluator<T, S> &evl, MovementGenerator<T> &mg, int k);
    std::optional<std::reference_wrapper<Movement<T>>> get_movement(const T &s) override;
};

template <typename T, typename S>
class NEFindFirst : public NeighborhoodExplorationMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    NEFindFirst(Evaluator<T, S> &evl, MovementGenerator<T> &mg);
    std::optional<std::reference_wrapper<Movement<T>>> get_movement(const T &s) override;
};

template <typename T, typename S>
class NEFindNext : public NeighborhoodExplorationMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int j;
public:
    NEFindNext(Evaluator<T, S> &evl, MovementGenerator<T> &mg, int j);
    std::optional<std::reference_wrapper<Movement<T>>> get_movement(const T &s) override;
};

template <typename T, typename S>
class NEFindBest : public NeighborhoodExplorationMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    NEFindBest(Evaluator<T, S> &evl, MovementGenerator<T> &mg);
    std::optional<std::reference_wrapper<Movement<T>>> get_movement(const T &s) override;
};

template <typename T>
std::vector<T> all_neighbors(T &s, MovementGenerator<T> &mg);

template <typename T, typename S>
class RefinementHeuristicsMethod {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    Evaluator<T, S> &evl;
    MovementGenerator<T> &mg;
    RefinementHeuristicsMethod(Evaluator<T, S> &evl, MovementGenerator<T> &mg);
    virtual std::optional<T> run(const T &s) = 0;
};

template <typename T, typename S>
class RHRandomSelection : public RefinementHeuristicsMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int k;
public:
    RHRandomSelection(Evaluator<T, S> &evl, MovementGenerator<T> &mg, int k);
    std::optional<T> run(const T &s) override;
};

template <typename T, typename S>
class RHFirstImprovement : public RefinementHeuristicsMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    RHFirstImprovement(Evaluator<T, S> &evl, MovementGenerator<T> &mg);
    std::optional<T> run(const T &s) override;
};

template <typename T, typename S>
class RHBestImprovement : public RefinementHeuristicsMethod<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    RHBestImprovement(Evaluator<T, S> &evl, MovementGenerator<T> &mg);
    std::optional<T> run(const T &s) override;
};

template <typename T, typename S>
class LocalSearch {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    Evaluator<T, S> &evl;
    RefinementHeuristicsMethod<T, S> &rh;
    LocalSearch(Evaluator<T, S> &evl, RefinementHeuristicsMethod<T, S> &rh);
    virtual T run(const T &s) = 0;
};

template <typename T, typename S>
class LSHillClimbing : public LocalSearch<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    LSHillClimbing(Evaluator<T, S> &evl, RefinementHeuristicsMethod<T, S> &rh);
    T run(const T &s) override;
};

template <typename T, typename S>
class RandomDescentMethod : public LocalSearch<T, S> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int k;
public:
    RandomDescentMethod(Evaluator<T, S> &evl, RefinementHeuristicsMethod<T, S> &rh, int k);
    T run(const T &s) override;
};

#include "neighborhood_exploration.tpp"

#endif // NEIGHBORHOOD_EXPLORATION_H
