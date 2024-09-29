#include "meta_heuristics.h"

template <typename T>
MetaHeuristicAlgorithm<T>::MetaHeuristicAlgorithm(Evaluator<T> &evl) : evl(evl) {}

template <typename T>
MHSimulatedAnnealing<T>::MHSimulatedAnnealing(
    Evaluator<T> &evl,
    MovementGenerator<T> &mg,
    double t,
    int SA_max,
    double a,
    double t_0,
    double t_min
)
    : MetaHeuristicAlgorithm<T>(evl), mg(mg), ne(evl, mg, 1)
{
    this->t = t;
    this->SA_max = SA_max;
    this->a = a;
    this->t_0 = t_0;
    this->t_min = t_min;
}

template <typename T>
T MHSimulatedAnnealing<T>::run(T &s) { 
    T s_prime = s;
    long long s_prime_value = this->evl.get_evaluation(s_prime);
    T s_curr = s;
    long long s_curr_value = s_prime_value;

    double curr_t = this->t_0;
    while (curr_t > this->t_min) {
        for (int i=0; i<this->SA_max; i++) {
            auto m = this->ne.get_movement(s_curr);
            if (!m.has_value()) break;
            
            T s1 = m.value().get().move(s_curr);
            long long s1_value = this->evl.get_evaluation(s1);

            long long delta = s1_value - s_curr_value;
            if (delta > 0) {
                s_curr = s1;
                if (s1_value > s_prime_value) {
                    s_prime = s1;
                    s_prime_value = s1_value;
                }
            } else {
                double x = std::rand() / RAND_MAX;
                if (x < std::exp(-delta / curr_t)) {
                    s_curr = s1;
                }
            }
        }

        curr_t = this->a * curr_t;
    }

    return s_prime;
}
