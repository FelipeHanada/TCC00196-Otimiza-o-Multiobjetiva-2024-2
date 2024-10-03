#include "meta_heuristics.h"

template <class SolutionClass>
MetaHeuristicAlgorithm<SolutionClass>::MetaHeuristicAlgorithm(Evaluator<SolutionClass> *evl) {
    this->evl = evl;
}

template <class SolutionClass>
MHSimulatedAnnealing<SolutionClass>::MHSimulatedAnnealing(
    Evaluator<SolutionClass> *evl,
    MovementGenerator<SolutionClass> *mg,
    double t,
    int SA_max,
    double a,
    double t_0,
    double t_min
)
    : MetaHeuristicAlgorithm<SolutionClass>(evl), ne(evl, mg, 1)
{
    this->mg = mg;
    this->t = t;
    this->SA_max = SA_max;
    this->a = a;
    this->t_0 = t_0;
    this->t_min = t_min;
}

template <class SolutionClass>
SolutionClass MHSimulatedAnnealing<SolutionClass>::pre_heat(const SolutionClass &s, double & final_t) {
    // TO IMPLEMENT
}

template <class SolutionClass>
SolutionClass MHSimulatedAnnealing<SolutionClass>::run(const SolutionClass &s) {     
    SolutionClass s_prime = s;
    long long s_prime_value = this->evl->get_evaluation(s_prime);
    SolutionClass s_curr = s;
    long long s_curr_value = s_prime_value;

    double curr_t = this->t_0;

    auto start = std::chrono::high_resolution_clock::now();
    auto current = std::chrono::high_resolution_clock::now();
    while (curr_t > this->t_min && std::chrono::duration<float>(current - start).count() < t) {
        current = std::chrono::high_resolution_clock::now();

        for (int i=0; i<this->SA_max && std::chrono::duration<float>(current - start).count() < t; i++) {
            current = std::chrono::high_resolution_clock::now();

            Movement<SolutionClass> *m = this->ne.get_movement(s_curr);
            if (m == NULL) break;
            
            SolutionClass s1 = m->move(s_curr);
            long long s1_value = this->evl->get_evaluation(s1);

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
