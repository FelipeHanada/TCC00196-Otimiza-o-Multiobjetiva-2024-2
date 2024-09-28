#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <vector>
#include <map>
#include <optional>
#include <type_traits>

class Solution {
    bool evaluated;
    long long evaluation;
public:
    Solution();
    bool is_evaluated() const;
    void set_evaluated(bool e);
    long long get_evaluation() const;
    void set_evaluation(long long e);
};

template <typename T>
class Evaluator {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    virtual long long evaluate(const T &s) = 0;
public:
    long long get_evaluation(T &s) {
        if (!s.is_evaluated())
            s.set_evaluation(this->evaluate(s));

        return s.get_evaluation();
    };
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
