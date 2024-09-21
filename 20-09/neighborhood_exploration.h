#include "optimization.h"
#include <vector>

template <typename T>
class NeighborhoodExplorationMethod {
static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    Evaluator<T> &evl;
    MovementGenerator<T> &mg;
    NeighborhoodExplorationMethod(Evaluator<T> &evl, MovementGenerator<T> &mg)
        : evl(evl), mg(mg) {}
    virtual T run(T s) = 0;
};

template <typename T>
class NEFindAny : public NeighborhoodExplorationMethod<T> {
static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int k;
public:
    NEFindAny(Evaluator<T> &evl, MovementGenerator<T> &mg, int k)
        : NeighborhoodExplorationMethod<T>(evl, mg)
    {
        this->k = k;
    };
    T run(T s) override {
        std::vector<Movement<T>*> movements = this->mg.generate(s);

        T curr = s;
        int curr_value = this->evl.evaluate(s);
        for (int i=0; i<this->k && movements.size() > 0; i++) {
            int m = rand() % movements.size();
            T s1 = movements[m]->move(s);
            int s1_value = this->evl.evaluate(s1);
            if (s1_value > curr_value) {
                curr = s1;
                curr_value = s1_value;
            }
            movements.erase(
                std::remove(movements.begin(), movements.end(), movements[m]),
                movements.end()
            );
        }

        return curr;
    }
};

template <typename T>
class NEFindFirst : public NeighborhoodExplorationMethod<T> {
static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    NEFindFirst(Evaluator<T> &evl, MovementGenerator<T> &mg)
        : NeighborhoodExplorationMethod<T>(evl, mg)
    {};
    T run(T s) override {
        std::vector<Movement<T>*> movements = this->mg.generate(s);

        int value = this->evl.evaluate(s);
        for (Movement<T> *m : movements) {
            T s1 = m->move(s);
            int s1_value = this->evl.evaluate(s1);
            if (s1_value > value) return s1;
        }
        return s;
    }
};

template <typename T>
class NEFindNext : public NeighborhoodExplorationMethod<T> {
static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int j;
public:
    NEFindNext(Evaluator<T> &evl, MovementGenerator<T> &mg, int j)
        : NeighborhoodExplorationMethod<T>(evl, mg)
    {
        this->j = j;
    };
    T run(T s) override {
        std::vector<Movement<T>*> movements = this->mg.generate(s);

        int value = this->evl.evaluate(s);
        for (int i=j+1; i<movements.size(); i++) {
            T s1 = movements[i]->move(s);
            int s1_value = this->evl.evaluate(s1);
            if (s1_value > value) return s1;
        }
        return s;
    }
};

template <typename T>
class NEFindBest : public NeighborhoodExplorationMethod<T> {
static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    NEFindBest(Evaluator<T> &evl, MovementGenerator<T> &mg)
        : NeighborhoodExplorationMethod<T>(evl, mg)
    {};
    T run(T s) override {
        std::vector<Movement<T>*> movements = this->mg.generate(s);

        T curr = s;
        int curr_value = this->evl.evaluate(s);
        for (Movement<T> *m : movements) {
            T s1 = m->move(s);
            int s1_value = this->evl.evaluate(s1);
            if (s1_value > curr_value) {
                curr = s1;
                curr_value = s1_value;
            }
        }
        return curr;
    }
};

template <typename T>
std::vector<T> all_neighbors(T s, MovementGenerator<T> &mg) {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");

    std::vector<T> r;
    for (Movement<T> *m : mg.get(s)) {
        r.push_back(m->move(s));
    }
    return r;
}

template <typename T>
class LocalSearch {
static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    Evaluator<T> &evl;
    NeighborhoodExplorationMethod<T> &ne;
    LocalSearch(Evaluator<T> &evl, NeighborhoodExplorationMethod<T> &ne)
        : evl(evl), ne(ne) {}
    virtual T run(T s) = 0;
};

template <typename T>
class LSHillClimbing : public LocalSearch<T> {
static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    LSHillClimbing(Evaluator<T> &evl, NeighborhoodExplorationMethod<T> &ne)
        : LocalSearch<T>(evl, ne)
    {};
    T run(T s) override {
        T curr = s;
        int curr_value = this->evl.evaluate(s);
        while (true) {
            T s1 = this->ne.run(curr);
            int s1_value = this->evl.evaluate(s1);
            if (s1_value <= curr_value) break;
            curr = s1;
            curr_value = s1_value;
        }
        return curr;
    }
};

template <typename T>
class RandomDescentMethod : public LocalSearch<T> {
static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    int k;
public:
    RandomDescentMethod(Evaluator<T> &evl, NeighborhoodExplorationMethod<T> &ne, int k)
        : LocalSearch<T>(evl, ne)
    {
        this->k = k;
    };
    T run(T s) override {
        T curr = s;
        int curr_value = this->evl.evaluate(s);
        int curr_k = k;
        while (curr_k > 0) {
            T s1 = this->ne.run(curr);
            int s1_value = this->evl.evaluate(s1);
            if (s1_value > curr_value) {
                curr = s1;
                curr_value = s1_value;
                curr_k = k;            
            } else {
                curr_k--;
            }
        }
        return curr;
    }
};
