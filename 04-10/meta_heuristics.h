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
    NEFindAny<SolutionClass> ne;
    MovementGenerator<SolutionClass> *mg;
    int SA_max;
    double t_0;
    double t_min;
    double a;
public:
    MHSimulatedAnnealing(
        Evaluator<SolutionClass> *evl,
        MovementGenerator<SolutionClass> *mg,
        int SA_max,
        double a,
        double t_0,
        double t_min = 0.00001
    );
    SolutionClass* pre_heat(const SolutionClass *s, double &final_t);
    SolutionClass* run(const SolutionClass *s, double t) override;
};

#include "meta_heuristics.tpp"

#endif  // META_HEURISTICS_H
