#include "meta_heuristics.h"

template <class SolutionClass>
MetaHeuristicAlgorithm<SolutionClass>::MetaHeuristicAlgorithm(Evaluator<SolutionClass> *evl) {
    this->evl = evl;
}

template <class SolutionClass>
MHSimulatedAnnealing<SolutionClass>::MHSimulatedAnnealing(
    Evaluator<SolutionClass> *evl,
    MovementGenerator<SolutionClass> *mg,
    int SA_max,
    double a,
    double t_0,
    double t_min
)
    : MetaHeuristicAlgorithm<SolutionClass>(evl), ne(evl, mg, 1)
{
    this->mg = mg;
    this->SA_max = SA_max;
    this->a = a;
    this->t_0 = t_0;
    this->t_min = t_min;
}

template <class SolutionClass>
SolutionClass* MHSimulatedAnnealing<SolutionClass>::pre_heat(const SolutionClass *s, double & final_t) {
    // TO IMPLEMENT
}

template <class SolutionClass>
SolutionClass* MHSimulatedAnnealing<SolutionClass>::run(const SolutionClass *s, double t) {
    SolutionClass *s_prime = (SolutionClass*) s->clone();
    SolutionClass *s_curr = (SolutionClass*) s->clone();

    double curr_t = this->t_0;

    auto start = std::chrono::high_resolution_clock::now();
    auto current = std::chrono::high_resolution_clock::now();
    while (curr_t > this->t_min && std::chrono::duration<float>(current - start).count() < t) {
        current = std::chrono::high_resolution_clock::now();

        for (int i=0; i<this->SA_max && std::chrono::duration<float>(current - start).count() < t; i++) {
            current = std::chrono::high_resolution_clock::now();

            Movement<SolutionClass> *m = this->ne.get_movement(s_curr);
            if (m == NULL) break;
            
            long long delta = m->delta(s_curr);
            if (delta > 0 || std::rand() / RAND_MAX < std::exp(delta / curr_t)) {
                SolutionClass *s1 = (SolutionClass*) s_curr->clone();
                delete s_curr;
                m->move(s1);
                s_curr = s1;

                if (this->evl->get_evaluation(s_curr) > this->evl->get_evaluation(s_prime)) {
                    delete s_prime;
                    s_prime = (SolutionClass*) s_curr->clone();
                }
            }

            delete m;
        }

        curr_t = this->a * curr_t;
    }

    delete s_curr;
    return s_prime;
}
