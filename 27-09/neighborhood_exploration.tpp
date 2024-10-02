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
std::optional<std::reference_wrapper<Movement<SolutionClass>>> NEFindAny<SolutionClass>::get_movement(const SolutionClass &s) {
    std::vector<Movement<SolutionClass>*> movements = this->mg->generate(s);

    SolutionClass curr(s);
    std::optional<std::reference_wrapper<Movement<SolutionClass>>> curr_m = std::nullopt;
    long long curr_value = this->evl->get_evaluation(s);
    for (int i = 0; i < this->k && movements.size() > 0; i++) {
        int r = std::rand() % movements.size();
        Movement<SolutionClass> *m = movements[r];
        SolutionClass s1 = m->move(s);
        long long s1_value = this->evl->get_evaluation(s1);
        if (s1_value > curr_value) {
            curr = s1;
            curr_m = *m;
            curr_value = s1_value;
        }
        movements.erase(
            std::remove(movements.begin(), movements.end(), movements[r]),
            movements.end()
        );
    }

    return curr_m;
}

template <typename SolutionClass>
NEFindFirst<SolutionClass>::NEFindFirst(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg) {}

template <typename SolutionClass>
std::optional<std::reference_wrapper<Movement<SolutionClass>>> NEFindFirst<SolutionClass>::get_movement(const SolutionClass &s) {
    std::vector<Movement<SolutionClass>*> movements = this->mg->generate(s);

    long long value = this->evl->get_evaluation(s);
    for (Movement<SolutionClass> *m : movements) {
        SolutionClass s1 = m->move(s);
        long long s1_value = this->evl->get_evaluation(s1);
        if (s1_value > value) {
            return *m;
        }
    }
    return std::nullopt;
}

template <typename SolutionClass>
NEFindNext<SolutionClass>::NEFindNext(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg, int j)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg), j(j) {}

template <typename SolutionClass>
std::optional<std::reference_wrapper<Movement<SolutionClass>>> NEFindNext<SolutionClass>::get_movement(const SolutionClass &s) {
    std::vector<Movement<SolutionClass>*> movements = this->mg->generate(s);

    long long value = this->evl->get_evaluation(s);
    for (int i = j + 1; i < movements.size(); i++) {
        Movement<SolutionClass> m = movements[i];
        SolutionClass s1 = m->move(s);
        long long s1_value = this->evl->get_evaluation(s1);
        if (s1_value > value) return m;
    }
    return std::nullopt;
}

template <typename SolutionClass>
NEFindBest<SolutionClass>::NEFindBest(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : NeighborhoodExplorationMethod<SolutionClass>(evl, mg) {}

template <typename SolutionClass>
std::optional<std::reference_wrapper<Movement<SolutionClass>>> NEFindBest<SolutionClass>::get_movement(const SolutionClass &s) {
    std::vector<Movement<SolutionClass>*> movements = this->mg->generate(s);

    SolutionClass curr(s);
    std::optional<std::reference_wrapper<Movement<SolutionClass>>> curr_m = std::nullopt;
    long long curr_value = this->evl->get_evaluation(s);
    for (Movement<SolutionClass> *m : movements) {
        SolutionClass s1 = m->move(s);
        long long s1_value = this->evl->get_evaluation(s1);
        if (s1_value > curr_value) {
            curr = s1;
            curr_m = *m;
            curr_value = s1_value;
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
    std::optional<std::reference_wrapper<Movement<SolutionClass>>> m = ne.get_movement(s);

    if (m.has_value()) {
        Movement<SolutionClass> &m1 = m.value().get();
        SolutionClass s1 = m1.move(s);
        return s1;
    }

    return std::nullopt;
}

template <typename SolutionClass>
RHFirstImprovement<SolutionClass>::RHFirstImprovement(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : RefinementHeuristicsMethod<SolutionClass>(evl, mg) {}

template <typename SolutionClass>
std::optional<SolutionClass> RHFirstImprovement<SolutionClass>::run(const SolutionClass &s) {
    NEFindFirst<SolutionClass> ne(this->evl, this->mg);
    std::optional<std::reference_wrapper<Movement<SolutionClass>>> m = ne.get_movement(s);

    if (m.has_value()) {
        Movement<SolutionClass> &m1 = m.value().get();
        SolutionClass s1 = m1.move(s);
        return s1;
    }

    return std::nullopt;
}

template <typename SolutionClass>
RHBestImprovement<SolutionClass>::RHBestImprovement(Evaluator<SolutionClass> *evl, MovementGenerator<SolutionClass> *mg)
    : RefinementHeuristicsMethod<SolutionClass>(evl, mg) {}

template <typename SolutionClass>
std::optional<SolutionClass> RHBestImprovement<SolutionClass>::run(const SolutionClass &s) {
    NEFindBest<SolutionClass> ne(this->evl, this->mg);
    std::optional<std::reference_wrapper<Movement<SolutionClass>>> m = ne.get_movement(s);

    if (m.has_value()) {
        Movement<SolutionClass> &m1 = m.value().get();
        SolutionClass s1 = m1.move(s);
        return s1;
    }

    return std::nullopt;
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
