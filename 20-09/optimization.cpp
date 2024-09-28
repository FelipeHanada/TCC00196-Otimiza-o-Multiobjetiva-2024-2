#include "optimization.h"

Solution::Solution() {
    this->evaluated = false;
    this->last_evaluation = 0;
}

bool Solution::is_evaluated() const {
    return this->evaluated;
}

void Solution::set_evaluated(bool e) {
    this->evaluated = e;
}

long long Solution::get_last_evaluation() const {
    return this->last_evaluation;
}

void Solution::set_evaluation(long long e) {
    this->last_evaluation = e;
    this->set_evaluated(true);
}
