#ifndef _UTIL_HH_
#define _UTIL_HH_ 

#include <string>
#include <queue>
#include <list>
#include <utility>

#include <gmpxx.h>

namespace euler {
typedef unsigned long ulong;
typedef mpz_class bigint;
typedef std::string (*problem_fun)();
typedef std::pair<ulong, std::string> answer;

extern problem_fun NYI_problem;

std::vector<ulong> factors(ulong x);
std::vector<std::pair<ulong, unsigned char>> ct_factors(ulong x);
ulong divisor_ct(ulong x);

class problem {
    ulong number;
    problem_fun fun;
public:
    problem(ulong n = 0, problem_fun f = NYI_problem)
	: number(n), fun(f) {}
    ulong get_number() const { return number; }
    problem_fun get_fun() const { return fun; }
    std::string operator()() const {
	return (*fun)();
    }
    bool operator<(problem const& other) const {
	return number < other.number;
    }
};
}

#endif
