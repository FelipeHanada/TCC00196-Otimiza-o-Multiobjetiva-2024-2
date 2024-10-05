#include "neighborhood_exploration.h"

template <class SolutionClass>
NeighborhoodExplorationMethod<SolutionClass>::NeighborhoodExplorationMethod(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg) {
    this->evl = evl;
    this->mg = mg;
}

template <class SolutionClass>
NEFindAny<SolutionClass>::NEFindAny(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg, int k)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg), k(k) {}

template <class SolutionClass>
Movement<SolutionClass>* NEFindAny<SolutionClass>::get_movement(const SolutionClass *s) {
    Movement<SolutionClass>* curr_m = NULL;
    long long curr_value = this->evl->get_evaluation(s);
    
    for (int i = 0; i < this->k; i++) {
        Movement<SolutionClass> *m = this->mg->get_random();

        SolutionClass *s1 = (SolutionClass*) s->clone();
        m->move(s1);
        long long s1_value = this->evl->get_evaluation(s1);
        delete s1;

        if (s1_value > curr_value) {
            delete curr_m;
            curr_m = m;
            curr_value = s1_value;
        } else {
            delete m;
        }
    }

    return curr_m;
}

template <class SolutionClass>
NEFindFirst<SolutionClass>::NEFindFirst(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg) {}

template <class SolutionClass>
Movement<SolutionClass>* NEFindFirst<SolutionClass>::get_movement(const SolutionClass *s) {
    long long value = this->evl->get_evaluation(s);

    this->mg->reset();
    while (this->mg->has_next()) {
        Movement<SolutionClass> *m = this->mg->next();

        SolutionClass *s1 = (SolutionClass*) s->clone();
        m->move(s1);
        long long s1_value = this->evl->get_evaluation(s1);
        delete s1;
        
        if (s1_value > value) {
            return m;
        }
        delete m;
    }

    return NULL;
}

template <class SolutionClass>
NEFindNext<SolutionClass>::NEFindNext(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg, int j)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg), j(j) {}

template <class SolutionClass>
Movement<SolutionClass>* NEFindNext<SolutionClass>::get_movement(const SolutionClass *s) {
    this->mg->reset();
    for (int i = 0; i <= this->j; i++)
        delete this->mg->next();

    long long value = this->evl->get_evaluation(s);
    while (this->mg->has_next()) {
        Movement<SolutionClass> *m = this->mg->next();
        
        SolutionClass *s1 = (SolutionClass*) s->clone();
        m->move(s1);
        long long s1_value = this->evl->get_evaluation(s1);
        delete s1;
        
        if (s1_value > value) {
            return m;
        }
        delete m;
    }

    return NULL;
}

template <class SolutionClass>
NEFindBest<SolutionClass>::NEFindBest(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg) {}

template <class SolutionClass>
Movement<SolutionClass>* NEFindBest<SolutionClass>::get_movement(const SolutionClass *s) {
    this->mg->reset();

    Movement<SolutionClass>* curr_m = NULL;
    long long curr_value = this->evl->get_evaluation(s);
    while (this->mg->has_next()) {
        Movement<SolutionClass> *m = this->mg->next();

        SolutionClass *s1 = (SolutionClass*) s->clone();
        m->move(s1);
        long long s1_value = this->evl->get_evaluation(s1);
        delete s1;
        
        if (s1_value > curr_value) {
            delete curr_m;
            curr_m = m;
            curr_value = s1_value;
        } else {
            delete m;
        }
    }

    return curr_m;
}

template <class SolutionClass>
std::vector<SolutionClass> all_neighbors(SolutionClass &s, MovementGenerator<SolutionClass> *mg) {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");

    std::vector<SolutionClass> r;
    for (Movement<SolutionClass> *m : mg->get(s)) {
        r.push_back(m->move(s));
        delete m;
    }

    return r;
}

template <class SolutionClass>
RefinementHeuristicsMethod<SolutionClass>::RefinementHeuristicsMethod(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg) {
    this->evl = evl;
    this->mg = mg;
}

template <class SolutionClass>
RHRandomSelection<SolutionClass>::RHRandomSelection(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg, int k)
    : RefinementHeuristicsMethod<SolutionClass>(evl, mg), k(k) {}

template <class SolutionClass>
SolutionClass* RHRandomSelection<SolutionClass>::run(const SolutionClass *s) {
    NEFindAny<SolutionClass> ne(this->evl, this->mg, this->k);
    Movement<SolutionClass>* m = ne.get_movement(s);

    if (m == NULL) return NULL;

    SolutionClass *s1 = (SolutionClass*) s->clone();
    m->move(s1);
    delete m;

    return s1;
}

template <class SolutionClass>
RHFirstImprovement<SolutionClass>::RHFirstImprovement(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : RefinementHeuristicsMethod<SolutionClass>(evl, mg) {}

template <class SolutionClass>
SolutionClass* RHFirstImprovement<SolutionClass>::run(const SolutionClass *s) {
    NEFindFirst<SolutionClass> ne(this->evl, this->mg);
    Movement<SolutionClass>* m = ne.get_movement(s);

    if (m == NULL) return NULL;
    
    SolutionClass *s1 = (SolutionClass*) s->clone();
    m->move(s1);
    delete m;

    return s1;
}

template <class SolutionClass>
RHBestImprovement<SolutionClass>::RHBestImprovement(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : RefinementHeuristicsMethod<SolutionClass>(evl, mg) {}

template <class SolutionClass>
SolutionClass* RHBestImprovement<SolutionClass>::run(const SolutionClass *s) {
    NEFindBest<SolutionClass> ne(this->evl, this->mg);
    Movement<SolutionClass>* m = ne.get_movement(s);

    if (m == NULL) return NULL;
    
    SolutionClass *s1 = (SolutionClass*) s->clone();
    m->move(s1);
    delete m;
    return s1;
}

template <class SolutionClass>
LocalSearch<SolutionClass>::LocalSearch(Evaluator<SolutionClass> *evl, RefinementHeuristicsMethod<SolutionClass> *rh) {
    this->evl = evl;
    this->rh = rh;
}

template <class SolutionClass>
LSHillClimbing<SolutionClass>::LSHillClimbing(Evaluator<SolutionClass> *evl, RefinementHeuristicsMethod<SolutionClass> *rh)
    : LocalSearch<SolutionClass>(evl, rh) {}

template <class SolutionClass>
SolutionClass* LSHillClimbing<SolutionClass>::run(const SolutionClass *s, float t) {
    auto start = std::chrono::high_resolution_clock::now();

    SolutionClass *curr = (SolutionClass*) s->clone();

    while (true) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<float>(current - start).count() > t)
            break;

        SolutionClass *s1 = this->rh->run(curr);
        if (s1 == NULL) break;

        delete curr;
        curr = s1;        
    }

    return curr;
}

template <class SolutionClass>
RandomDescentMethod<SolutionClass>::RandomDescentMethod(Evaluator<SolutionClass> *evl, RefinementHeuristicsMethod<SolutionClass> *rh, int k)
    : LocalSearch<SolutionClass>(evl, rh), k(k) {}

template <class SolutionClass>
SolutionClass* RandomDescentMethod<SolutionClass>::run(const SolutionClass *s, float t) {
    auto start = std::chrono::high_resolution_clock::now();

    SolutionClass *curr = (SolutionClass*) s->clone();
    int curr_k = this->k;
    while (curr_k > 0) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<float>(current - start).count() > t)
            break;

        SolutionClass *s1 = this->rh->run(curr);
        if (s1 == NULL) break;

        if (this->evl->get_evaluation(s1) > this->evl->get_evaluation(curr)) {
            delete curr;
            curr = s1;
            curr_k = k;            
        } else {
            curr_k--;
        }
    }
    return curr;
}
