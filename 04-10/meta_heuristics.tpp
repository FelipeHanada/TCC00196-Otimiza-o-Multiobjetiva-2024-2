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
    double b,
    double g,
    double t_min
)
    : MetaHeuristicAlgorithm<SolutionClass>(evl)
{
    if (a <= 0 || a >= 1)
        throw std::invalid_argument("Parameter 'a' must be greater than 0 and less than 1.");

    if (b <= 1)
        throw std::invalid_argument("Parameter 'b' must be greater than 1.");

    if (g <= 0 || g >= 1)
        throw std::invalid_argument("Parameter 'g' must be greater than 0 and less than 1.");

    this->mg = mg;
    this->SA_max = SA_max;
    this->a = a;
    this->b = b;
    this->g = g;
    this->t_min = t_min;
}

template <class SolutionClass>
double MHSimulatedAnnealing<SolutionClass>::initial_temperature(const SolutionClass *s, double b, double g) {
    double curr_t = this->t_min;

    while (true) {
        int curr_accepted = 0;

        for (int i=0; i<this->SA_max; i++) {
            Movement<SolutionClass> *m = this->mg->get_random();

            long long delta = m->delta(s);
            if (delta > 0 || std::rand() / RAND_MAX < std::exp(delta / curr_t)) {
                curr_accepted++;
            }
        }        

        if (curr_accepted > this->SA_max * g)
            break;

        curr_t = b * curr_t;
    }

    return curr_t;
}

template <class SolutionClass>
SolutionClass* MHSimulatedAnnealing<SolutionClass>::run(const SolutionClass *s, double t) {
    SolutionClass *s_prime = (SolutionClass*) s->clone();
    SolutionClass *s_curr = (SolutionClass*) s->clone();

    double curr_t = this->initial_temperature(s, this->b, this->g);

    auto start = std::chrono::high_resolution_clock::now();
    auto current = std::chrono::high_resolution_clock::now();
    while (curr_t > this->t_min && std::chrono::duration<float>(current - start).count() < t) {
        current = std::chrono::high_resolution_clock::now();

        for (int i=0; i<this->SA_max && std::chrono::duration<float>(current - start).count() < t; i++) {
            current = std::chrono::high_resolution_clock::now();

            Movement<SolutionClass> *m = this->mg->get_random();
            if (m == NULL) break;
            
            long long delta = m->delta(s_curr);
            if (delta > 0 || std::rand() / RAND_MAX < std::exp(delta / curr_t)) {
                SolutionClass *s1 = (SolutionClass*) s_curr->clone();
                m->move(s1);
                delete s_curr;
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
