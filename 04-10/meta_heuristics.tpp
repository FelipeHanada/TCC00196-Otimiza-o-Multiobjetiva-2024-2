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
    double alpha,
    double beta,
    double gamma,
    double t_min
)
    : MetaHeuristicAlgorithm<SolutionClass>(evl)
{
    if (alpha <= 0 || alpha >= 1)
        throw std::invalid_argument("Parameter 'alpha' must be greater than 0 and less than 1.");

    if (beta <= 1)
        throw std::invalid_argument("Parameter 'beta' must be greater than 1.");

    if (gamma <= 0 || gamma >= 1)
        throw std::invalid_argument("Parameter 'gamma' must be greater than 0 and less than 1.");

    this->mg = mg;
    this->SA_max = SA_max;
    this->alpha = alpha;
    this->beta = beta;
    this->gamma = gamma;
    this->t_min = t_min;
}

template <class SolutionClass>
double MHSimulatedAnnealing<SolutionClass>::initial_temperature(const SolutionClass *s) {
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

        if (curr_accepted > this->SA_max * gamma)
            break;

        curr_t = beta * curr_t;
    }

    return curr_t;
}

template <class SolutionClass>
SolutionClass* MHSimulatedAnnealing<SolutionClass>::run(const SolutionClass *s, double t) {
    SolutionClass *s_prime = (SolutionClass*) s->clone();
    SolutionClass *s_curr = (SolutionClass*) s->clone();

    double curr_t = this->initial_temperature(s, this->beta, this->gamma);

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

        curr_t = this->alpha * curr_t;
    }

    delete s_curr;
    return s_prime;
}

template <class SolutionClass>
MHGrasp<SolutionClass>::MHGrasp(
    Evaluator<SolutionClass> *evl,
    MovementGenerator<SolutionClass> *mg,
    std::function<SolutionClass*(Evaluator<SolutionClass>, double)> constructive_method,
    double alpha,
    LocalSearch<SolutionClass> *ls,
    int GRASP_max
)
    : MetaHeuristicAlgorithm<SolutionClass>(evl)
{
    this->mg = mg;
    this->constructive_method = constructive_method;
    this->alpha = alpha;
    this->ls = ls;
    this->GRASP_max = GRASP_max;
}

template <class SolutionClass>
SolutionClass* MHGrasp<SolutionClass>::run(const SolutionClass *s, double t) {
    SolutionClass *s_prime = (SolutionClass*) s->clone();

    auto start = std::chrono::high_resolution_clock::now();
    for (int i=0; i<this->GRASP_max; i++) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<float>(current - start).count() >= t) break;

        SolutionClass *s_curr = this->constructive_method(this->evl, this->alpha);
        SolutionClass *s1 = this->ls->run(s_curr, t);
        delete s_curr;

        if (this->evl->get_evaluation(s1) > this->evl->get_evaluation(s_prime)) {
            delete s_prime;
            s_prime = s1;
        } else {
            delete s1;
        }
    }

    return s_prime;
}
