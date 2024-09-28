#include "optimization.h"

template <typename T>
long long Evaluator<T>::get_evaluation(T &s) {
    if (!s.is_evaluated())
        s.set_evaluation(this->evaluate(s));

    return s.get_last_evaluation();
};

template <typename T>
void Evaluator<T>::clear_evaluation(T &s) {
    s.set_evaluated(false);
}

template <typename T>
void Evaluator<T>::set_evaluation(T &s, long long e) {
    s.set_evaluation(e);
}
