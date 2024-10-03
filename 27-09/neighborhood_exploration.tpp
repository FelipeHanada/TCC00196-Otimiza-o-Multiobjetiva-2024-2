#include "neighborhood_exploration.h"

template <typename SolutionClass>
NeighborhoodExplorationMethod<SolutionClass>::NeighborhoodExplorationMethod(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg) {
    this->evl = evl;
    this->mg = mg;
}

template <typename SolutionClass>
NEFindAny<SolutionClass>::NEFindAny(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg, int k)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg), k(k) {}

template <typename SolutionClass>
Movement<SolutionClass>* NEFindAny<SolutionClass>::get_movement(const SolutionClass &s) {
    std::vector<Movement<SolutionClass>*> movements = this->mg->generate(s);
    std::vector<int> indexes(movements.size());
    std::iota(indexes.begin(), indexes.end(), 0);

    SolutionClass curr(s);
    Movement<SolutionClass>* curr_m = NULL;
    long long curr_value = this->evl->get_evaluation(s);
    for (int i = 0; i < this->k && indexes.size() > 0; i++) {
        int r = indexes[std::rand() % indexes.size()];
        indexes.erase(
            std::remove(indexes.begin(), indexes.end(), r),
            indexes.end()
        );
        
        Movement<SolutionClass> *m = movements[r];
        SolutionClass s1 = m->move(s);
        long long s1_value = this->evl->get_evaluation(s1);
        if (s1_value > curr_value) {
            delete curr_m;
            curr = s1;
            curr_m = m;
            curr_value = s1_value;
        } else {
            delete m;
        }
    }

    for (int i : indexes)
        delete movements[i];

    return curr_m;
}

template <typename SolutionClass>
NEFindFirst<SolutionClass>::NEFindFirst(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg) {}

template <typename SolutionClass>
Movement<SolutionClass>* NEFindFirst<SolutionClass>::get_movement(const SolutionClass &s) {
    std::vector<Movement<SolutionClass>*> movements = this->mg->generate(s);

    long long value = this->evl->get_evaluation(s);
    Movement<SolutionClass>* m = NULL;
    for (int i = 0; i < movements.size(); i++) {
        Movement<SolutionClass> *movement = movements[i];
        SolutionClass s1 = movement->move(s);
        long long s1_value = this->evl->get_evaluation(s1);
        if (m == NULL && s1_value > value) {
            m = movement;
            continue;
        }
        delete movement;
    }
    
    return NULL;
}

template <typename SolutionClass>
NEFindNext<SolutionClass>::NEFindNext(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg, int j)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg), j(j) {}

template <typename SolutionClass>
Movement<SolutionClass>* NEFindNext<SolutionClass>::get_movement(const SolutionClass &s) {
    std::vector<Movement<SolutionClass>*> movements = this->mg->generate(s);

    for (int i=0; i <= this->j && i < movements.size(); i++)
        delete movements[i];

    long long value = this->evl->get_evaluation(s);
    Movement<SolutionClass>* m = NULL;
    for (int i = this->j + 1; i < movements.size(); i++) {
        Movement<SolutionClass> *movement = movements[i];
        SolutionClass s1 = movement->move(s);
        long long s1_value = this->evl->get_evaluation(s1);
        if (m == NULL && s1_value > value) {
            m = movement;
            continue;
        }
        delete movement;
    }
    
    return NULL;
}

template <typename SolutionClass>
NEFindBest<SolutionClass>::NEFindBest(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg) {}

template <typename SolutionClass>
Movement<SolutionClass>* NEFindBest<SolutionClass>::get_movement(const SolutionClass &s) {
    std::vector<Movement<SolutionClass>*> movements = this->mg->generate(s);

    SolutionClass curr(s);
    Movement<SolutionClass>* curr_m = NULL;
    long long curr_value = this->evl->get_evaluation(s);
    for (Movement<SolutionClass> *m : movements) {
        SolutionClass s1 = m->move(s);
        long long s1_value = this->evl->get_evaluation(s1);
        if (s1_value > curr_value) {
            delete curr_m;
            curr = s1;
            curr_m = m;
            curr_value = s1_value;
        } else {
            delete m;
        }
    }

    return curr_m;
}

template <typename SolutionClass>
std::vector<SolutionClass> all_neighbors(SolutionClass &s, MovementGenerator<SolutionClass> *mg) {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");

    std::vector<SolutionClass> r;
    for (Movement<SolutionClass> *m : mg->get(s)) {
        r.push_back(m->move(s));
        delete m;
    }

    return r;
}

template <typename SolutionClass>
RefinementHeuristicsMethod<SolutionClass>::RefinementHeuristicsMethod(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg) {
    this->evl = evl;
    this->mg = mg;
}

template <typename SolutionClass>
RHRandomSelection<SolutionClass>::RHRandomSelection(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg, int k)
    : RefinementHeuristicsMethod<SolutionClass>(evl, mg), k(k) {}

template <typename SolutionClass>
std::optional<SolutionClass> RHRandomSelection<SolutionClass>::run(const SolutionClass &s) {
    NEFindAny<SolutionClass> ne(this->evl, this->mg, this->k);
    Movement<SolutionClass>* m = ne.get_movement(s);

    if (m == NULL) return NULL;

    SolutionClass s1 = m->move(s);
    delete m;

    return s1;
}

template <typename SolutionClass>
RHFirstImprovement<SolutionClass>::RHFirstImprovement(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : RefinementHeuristicsMethod<SolutionClass>(evl, mg) {}

template <typename SolutionClass>
std::optional<SolutionClass> RHFirstImprovement<SolutionClass>::run(const SolutionClass &s) {
    NEFindFirst<SolutionClass> ne(this->evl, this->mg);
    Movement<SolutionClass>* m = ne.get_movement(s);

    if (m == NULL) return NULL;
    
    SolutionClass s1 = m->move(s);
    delete m;

    return s1;
}

template <typename SolutionClass>
RHBestImprovement<SolutionClass>::RHBestImprovement(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : RefinementHeuristicsMethod<SolutionClass>(evl, mg) {}

template <typename SolutionClass>
std::optional<SolutionClass> RHBestImprovement<SolutionClass>::run(const SolutionClass &s) {
    NEFindBest<SolutionClass> ne(this->evl, this->mg);
    Movement<SolutionClass>* m = ne.get_movement(s);

    if (m == NULL) return NULL;
    
    SolutionClass s1 = m->move(s);
    delete m;

    return s1;
}

template <typename SolutionClass>
LocalSearch<SolutionClass>::LocalSearch(Evaluator<SolutionClass> *evl, RefinementHeuristicsMethod<SolutionClass> *rh) {
    this->evl = evl;
    this->rh = rh;
}

template <typename SolutionClass>
LSHillClimbing<SolutionClass>::LSHillClimbing(Evaluator<SolutionClass> *evl, RefinementHeuristicsMethod<SolutionClass> *rh)
    : LocalSearch<SolutionClass>(evl, rh) {}

template <typename SolutionClass>
SolutionClass LSHillClimbing<SolutionClass>::run(const SolutionClass &s, float t) {
    auto start = std::chrono::high_resolution_clock::now();

    SolutionClass curr = s;
    long long curr_value = this->evl->get_evaluation(s);
    while (true) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<float>(current - start).count() > t)
            break;

        std::optional<SolutionClass> s1 = this->rh->run(curr);
        if (!s1.has_value()) break;
        
        long long s1_value = this->evl->get_evaluation(s1.value());
        if (s1_value <= curr_value) break;
        curr = s1.value();
        curr_value = s1_value;
    }
    return curr;
}

template <typename SolutionClass>
RandomDescentMethod<SolutionClass>::RandomDescentMethod(Evaluator<SolutionClass> *evl, RefinementHeuristicsMethod<SolutionClass> *rh, int k)
    : LocalSearch<SolutionClass>(evl, rh), k(k) {}

template <typename SolutionClass>
SolutionClass RandomDescentMethod<SolutionClass>::run(const SolutionClass &s, float t) {
    auto start = std::chrono::high_resolution_clock::now();

    SolutionClass curr = s;
    long long curr_value = this->evl->get_evaluation(s);
    int curr_k = this->k;
    while (curr_k > 0) {
        auto current = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration<float>(current - start).count() > t)
            break;

        std::optional<SolutionClass> s1 = this->rh->run(curr);
        if (!s1.has_value()) break;

        long long s1_value = this->evl->get_evaluation(s1.value());
        if (s1_value > curr_value) {
            curr = s1.value();
            curr_value = s1_value;
            curr_k = k;            
        } else {
            curr_k--;
        }
    }
    return curr;
}
