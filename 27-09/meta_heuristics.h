#ifndef META_HEURISTICS_H
#define META_HEURISTICS_H

#include <cmath>
#include "optimization.h"
#include "neighborhood_exploration.h"

template <typename T>
class MetaHeuristicAlgorithm {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
protected:
    Evaluator<T> &evl;
public:
    MetaHeuristicAlgorithm(Evaluator<T> &evl);
    virtual T run(T &s) = 0;
};

template <typename T>
class MHSimulatedAnnealing : public MetaHeuristicAlgorithm<T> {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    NEFindAny<T> ne;
    MovementGenerator<T> &mg;
    double t;
    int SA_max;
    double t_0;
    double t_min;
    double a;
public:
    MHSimulatedAnnealing(
        Evaluator<T> &evl,
        MovementGenerator<T> &mg,
        double t,
        int SA_max,
        double a,
        double t_0,
        double t_min = 0.00001
    );
    T run(T &s) override;
};

#include "meta_heuristics.tpp"

#endif  // META_HEURISTICS_H
