#include "optimization.hpp"

Solution::Solution() {
    this->evaluated = false;
    this->last_evaluation = 0;
}

bool Solution::is_evaluated() const {
    return this->evaluated;
}

void Solution::set_evaluated(bool e) const {
    this->evaluated = e;
}

void Solution::clear_evaluation() const {
    this->set_evaluated(false);
}

long long Solution::get_last_evaluation() const {
    return this->last_evaluation;
}

void Solution::set_evaluation(long long e) const {
    this->last_evaluation = e;
    this->set_evaluated(true);
}
