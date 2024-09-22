#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <vector>
#include <type_traits>

class Solution {};

template <typename T, typename S>
class Evaluator {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    virtual S evaluate(const T &s) = 0;
};

template <typename T>
class Movement {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    virtual T move(const T &s) = 0;
};

template <typename T>
class MovementGenerator {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
public:
    virtual std::vector<Movement<T>*> generate(const T &s) = 0;
};

#endif // OPTIMIZATION_H
