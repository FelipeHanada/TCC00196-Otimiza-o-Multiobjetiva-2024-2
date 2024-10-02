#ifndef NEIGHBORHOOD_EXPLORATION_H
#define NEIGHBORHOOD_EXPLORATION_H

#include <vector>
#include <optional>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include "optimization.h"

template <typename SolutionClass>
class NeighborhoodExplorationMethod {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
public:
    Evaluator<SolutionClass> *evl;
    MovementGenerator<SolutionClass> *mg;
    NeighborhoodExplorationMethod(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg);
    virtual std::optional<std::reference_wrapper<Movement<SolutionClass>>> get_movement(const SolutionClass &s) = 0;
};

template <typename SolutionClass>
class NEFindAny : public NeighborhoodExplorationMethod<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
private:
    int k;
public:
    NEFindAny(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg, int k);
    std::optional<std::reference_wrapper<Movement<SolutionClass>>> get_movement(const SolutionClass &s) override;
};

template <typename SolutionClass>
class NEFindFirst : public NeighborhoodExplorationMethod<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
public:
    NEFindFirst(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg);
    std::optional<std::reference_wrapper<Movement<SolutionClass>>> get_movement(const SolutionClass &s) override;
};

template <typename SolutionClass>
class NEFindNext : public NeighborhoodExplorationMethod<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
private:
    int j;
public:
    NEFindNext(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg, int j);
    std::optional<std::reference_wrapper<Movement<SolutionClass>>> get_movement(const SolutionClass &s) override;
};

template <typename SolutionClass>
class NEFindBest : public NeighborhoodExplorationMethod<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
public:
    NEFindBest(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg);
    std::optional<std::reference_wrapper<Movement<SolutionClass>>> get_movement(const SolutionClass &s) override;
};

template <typename SolutionClass>
std::vector<SolutionClass> all_neighbors(SolutionClass &s, MovementGenerator<SolutionClass> *mg);

template <typename SolutionClass>
class RefinementHeuristicsMethod {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
public:
    Evaluator<SolutionClass> *evl;
    MovementGenerator<SolutionClass> *mg;
    RefinementHeuristicsMethod(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg);
    virtual std::optional<SolutionClass> run(const SolutionClass &s) = 0;
};

template <typename SolutionClass>
class RHRandomSelection : public RefinementHeuristicsMethod<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
private:
    int k;
public:
    RHRandomSelection(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg, int k);
    std::optional<SolutionClass> run(const SolutionClass &s) override;
};

template <typename SolutionClass>
class RHFirstImprovement : public RefinementHeuristicsMethod<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
public:
    RHFirstImprovement(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg);
    std::optional<SolutionClass> run(const SolutionClass &s) override;
};

template <typename SolutionClass>
class RHBestImprovement : public RefinementHeuristicsMethod<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
public:
    RHBestImprovement(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg);
    std::optional<SolutionClass> run(const SolutionClass &s) override;
};

template <typename SolutionClass>
class LocalSearch {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
public:
    Evaluator<SolutionClass> *evl;
    RefinementHeuristicsMethod<SolutionClass> *rh;
    LocalSearch(Evaluator<SolutionClass> *evl, RefinementHeuristicsMethod<SolutionClass> *rh);
    virtual SolutionClass run(const SolutionClass &s, float t) = 0;
};

template <typename SolutionClass>
class LSHillClimbing : public LocalSearch<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
public:
    LSHillClimbing(Evaluator<SolutionClass> *evl, RefinementHeuristicsMethod<SolutionClass> *rh);
    SolutionClass run(const SolutionClass &s, float t) override;
};

template <typename SolutionClass>
class RandomDescentMethod : public LocalSearch<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
private:
    int k;
public:
    RandomDescentMethod(Evaluator<SolutionClass> *evl, RefinementHeuristicsMethod<SolutionClass> *rh, int k);
    SolutionClass run(const SolutionClass &s, float t) override;
};

#include "neighborhood_exploration.tpp"

#endif // NEIGHBORHOOD_EXPLORATION_H
