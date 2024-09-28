#include "optimization.h"

Solution::Solution() {
    this->evaluated = false;
    this->evaluation = 0;
}

bool Solution::is_evaluated() const {
    return this->evaluated;
}

void Solution::set_evaluated(bool e) {
    this->evaluated = e;
}

long long Solution::get_evaluation() {
    return this->evaluation;
}

void Solution::set_evaluation(long long e) {
    this->evaluation = e;
    this->set_evaluated(true);
}
