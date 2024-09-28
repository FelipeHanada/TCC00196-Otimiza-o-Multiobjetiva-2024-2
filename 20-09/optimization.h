#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <vector>
#include <map>
#include <optional>
#include <type_traits>

template <typename T>
class Evaluator;

class Solution {
    bool evaluated;
    long long last_evaluation;
protected:
    bool is_evaluated() const;
    void set_evaluated(bool e);
    long long get_last_evaluation() const;
    void set_evaluation(long long e);
public:
    Solution();
    template <typename T>
    friend class Evaluator;
};

template <typename T>
class Evaluator {
    static_assert(std::is_base_of<Solution, T>::value, "T must be a descendant of Solution");
    virtual long long evaluate(const T &s) = 0;
public:
    long long get_evaluation(T &s) {
        if (!s.is_evaluated())
            s.set_evaluation(this->evaluate(s));

        return s.get_last_evaluation();
    };
    void clear_evaluation(T &s) {
        s.set_evaluated(false);
    }
    void set_evaluation(T &s, long long e) {
        s.set_evaluation(e);
    }
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
