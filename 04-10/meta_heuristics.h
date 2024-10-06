#ifndef META_HEURISTICS_H
#define META_HEURISTICS_H

#include <cmath>
#include "optimization.hpp"
#include "neighborhood_exploration.h"

template <class SolutionClass>
class MetaHeuristicAlgorithm {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
protected:
    Evaluator<SolutionClass> *evl;
public:
    MetaHeuristicAlgorithm(Evaluator<SolutionClass> *evl);
    virtual SolutionClass* run(double t) = 0;
};

template <class SolutionClass>
class MHSimulatedAnnealing : public MetaHeuristicAlgorithm<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
private:
    SolutionClass *s_0;
    MovementGenerator<SolutionClass> *mg;
    int SA_max;
    double alpha;
    double beta;
    double gamma;
    double t_min;
public:
    MHSimulatedAnnealing(
        Evaluator<SolutionClass> *evl,
        MovementGenerator<SolutionClass> *mg,
        SolutionClass *s_0,
        int SA_max,
        double alpha = 0.95,
        double beta = 1.05,
        double gamma = 0.9,
        double t_min = 0.00001
    );
    double initial_temperature(const SolutionClass *s);
    SolutionClass* run(double t) override;
};

template <class SolutionClass>
class MHGrasp : public MetaHeuristicAlgorithm<SolutionClass> {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
private:
    SolutionClass* (*constructive_method)(Evaluator<SolutionClass>*, double);
    double alpha;
    LocalSearch<SolutionClass> *ls;
    int GRASP_max;
public:
    MHGrasp(
        Evaluator<SolutionClass> *evl,
        SolutionClass* (*constructive_method)(Evaluator<SolutionClass>*, double),
        double alpha,
        LocalSearch<SolutionClass> *ls,
        int GRASP_max
    );
    SolutionClass* run(double t) override;
};

#include "meta_heuristics.tpp"

#endif  // META_HEURISTICS_H
