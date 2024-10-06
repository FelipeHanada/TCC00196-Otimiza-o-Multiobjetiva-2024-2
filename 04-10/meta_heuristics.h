#ifndef META_HEURISTICS_H
#define META_HEURISTICS_H

#include <cmath>
#include "optimization.hpp"
#include "neighborhood_exploration.h"

template <typename SolutionClass>
class MetaHeuristicAlgorithm {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
protected:
    Evaluator<SolutionClass> *evl;
public:
    MetaHeuristicAlgorithm(Evaluator<SolutionClass> *evl);
    virtual SolutionClass* run(const SolutionClass *s, double t) = 0;
};

template <typename SolutionClass>
class MHSimulatedAnnealing : public MetaHeuristicAlgorithm<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
    MovementGenerator<SolutionClass> *mg;
    int SA_max;
    double a;
    double b;
    double g;
    double t_min;
public:
    MHSimulatedAnnealing(
        Evaluator<SolutionClass> *evl,
        MovementGenerator<SolutionClass> *mg,
        int SA_max,
        double a = 0.95,
        double b = 1.05,
        double g = 0.9,
        double t_min = 0.00001
    );
    double initial_temperature(const SolutionClass *s, double b, double g);
    SolutionClass* run(const SolutionClass *s, double t) override;
};

#include "meta_heuristics.tpp"

#endif  // META_HEURISTICS_H
