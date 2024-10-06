#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <vector>
#include <map>
#include <optional>
#include <type_traits>

template <class SolutionClass>
class Evaluator;

class Solution {
private:
    mutable bool evaluated;
    mutable long long last_evaluation;
protected:
    bool is_evaluated() const;
    void set_evaluated(bool e) const;
    void clear_evaluation() const;
    long long get_last_evaluation() const;
    void set_evaluation(long long e) const;
public:
    Solution();
    virtual Solution* clone() const = 0;
    template <class SolutionClass>
    friend class Evaluator;
    template <class SolutionClass>
    friend class Movement;
};

template <class SolutionClass>
class Evaluator {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
protected:
    virtual long long evaluate(const SolutionClass *s) const = 0;
public:
    virtual long long get_evaluation(const SolutionClass *s) const;
};

template <class SolutionClass>
class Movement {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
public:
    virtual void move(SolutionClass *s) = 0;
    virtual long long delta(const SolutionClass *s) const = 0;
};

template <class SolutionClass>
class MovementGenerator {
    static_assert(std::is_base_of<Solution, SolutionClass>::value, "SolutionClass must be a descendant of Solution");
public:
    virtual std::vector<Movement<SolutionClass>*> get_all(const SolutionClass *s) = 0;
    virtual Movement<SolutionClass>* get_random() = 0;
    virtual bool has_next() = 0;
    virtual Movement<SolutionClass>* next() = 0;
    virtual void reset() = 0; 
};

#include "optimization.tpp"

#endif // OPTIMIZATION_H
