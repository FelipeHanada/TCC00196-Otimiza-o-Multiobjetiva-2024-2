#include "meta_heuristics.h"

template <class SolutionClass>
MetaHeuristicAlgorithm<SolutionClass>::MetaHeuristicAlgorithm(Evaluator<SolutionClass> *evl) {
    this->evl = evl;
}

template <class SolutionClass>
MHSimulatedAnnealing<SolutionClass>::MHSimulatedAnnealing(
    Evaluator<SolutionClass> *evl,
    MovementGenerator<SolutionClass> *mg,
    SolutionClass *s_0,
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
    this->s_0 = s_0;
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
            if (delta > 0 || std::rand() / (double) RAND_MAX < std::exp(delta / curr_t)) {
                curr_accepted++;
            }
        }        

        if (curr_accepted > this->SA_max * this->gamma)
            break;

        curr_t = this->beta * curr_t;
    }

    return curr_t;
}

template <class SolutionClass>
SolutionClass* MHSimulatedAnnealing<SolutionClass>::run(double t) {
    auto start = std::chrono::high_resolution_clock::now();

    SolutionClass *s_prime = (SolutionClass*) this->s_0->clone();
    SolutionClass *s_curr = (SolutionClass*) this->s_0->clone();

    double curr_t = this->initial_temperature(this->s_0);
    std::cout << "Simulated Annealing starting with t_0 = " << curr_t << "." << std::endl;

    while (curr_t > this->t_min) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<double>(current - start).count() >= t) {
            std::cout << "> Simulated Annealing finished by time." << std::endl;
            break;
        }

        for (int i=0; i<this->SA_max; i++) {
            current = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration<double>(current - start).count() >= t) break;            

            Movement<SolutionClass> *m = this->mg->get_random();
            if (m == NULL) break;
            
            long long delta = m->delta(s_curr);
            if (delta > 0 || std::rand() / (double) RAND_MAX < std::exp(delta / curr_t)) {
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

    auto current = std::chrono::high_resolution_clock::now();
    auto total_time = std::chrono::duration<double>(current - start).count();
    std::cout << "Simulated Annealing finished after " << total_time << " seconds." << std::endl;

    delete s_curr;
    return s_prime;
}

template <class SolutionClass>
MHGrasp<SolutionClass>::MHGrasp(
    Evaluator<SolutionClass> *evl,
    SolutionClass* (*constructive_method)(Evaluator<SolutionClass>*, double),
    double alpha,
    LocalSearch<SolutionClass> *ls,
    int GRASP_max
)
    : MetaHeuristicAlgorithm<SolutionClass>(evl)
{
    this->constructive_method = constructive_method;
    this->alpha = alpha;
    this->ls = ls;
    this->GRASP_max = GRASP_max;
}

template <class SolutionClass>
SolutionClass* MHGrasp<SolutionClass>::run(double t) {
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "GRASP starting." << std::endl;

    SolutionClass *s_tmp = this->constructive_method(this->evl, this->alpha);
    SolutionClass *s_prime = this->ls->run(s_tmp, t);
    delete s_tmp;

    int GRASP_curr = 0;
    for (; GRASP_curr<this->GRASP_max; GRASP_curr++) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<double>(current - start).count() >= t) {
            std::cout << "> GRASP finished by time." << std::endl;
            break;
        }

        s_tmp = this->constructive_method(this->evl, this->alpha);
        SolutionClass *s1 = this->ls->run(s_tmp, t);
        delete s_tmp;

        if (this->evl->get_evaluation(s1) > this->evl->get_evaluation(s_prime)) {
            delete s_prime;
            s_prime = s1;
        } else {
            delete s1;
        }
    }

    auto current = std::chrono::high_resolution_clock::now();
    auto total_time = std::chrono::duration<double>(current - start).count();
    std::cout << "GRASP finished after " << total_time << " seconds and " << GRASP_curr << " iterations." << std::endl;

    return s_prime;
}
