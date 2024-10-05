#include "optimization.h"

template <typename SolutionClass>
long long Evaluator<SolutionClass>::get_evaluation(const SolutionClass &s) const {
    if (!s.is_evaluated())
        s.set_evaluation(this->evaluate(s));

    return s.get_last_evaluation();
};
