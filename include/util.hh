#ifndef _UTIL_HH_
#define _UTIL_HH_ 

#include <string>
#include <queue>
#include <list>
#include <utility>
#include <stdint.h>

#include <gmpxx.h>

namespace euler {
typedef uint8_t uchar;
typedef uint32_t uint;
typedef uint64_t ulong;
typedef mpz_class bigint;
typedef std::string (*problem_fun)();
typedef std::pair<ulong, std::string> answer;

extern problem_fun NYI_problem;

bool is_palindrome(std::string const& str);

bool is_prime(ulong x);
std::vector<ulong> factors(ulong x);
std::vector<std::pair<ulong, unsigned char>> ct_factors(ulong x);

uint isqrt(ulong x);
uint isqrt_part(ulong x);

ulong divisor_ct(ulong x);
std::vector<ulong> divisors(ulong x);
ulong divisor_sum(ulong x);

class digit_iterator : public std::iterator<std::input_iterator_tag, unsigned char> {
    bigint source;
    unsigned char current;
public:
    digit_iterator() : source(0), current(0) {}
    explicit digit_iterator(bigint const& s) : source(s) {
	operator++();
    }
    unsigned char operator*() const {
	return current;
    }
    digit_iterator& operator++() noexcept {
	current = mpz_fdiv_q_ui(source.get_mpz_t(), source.get_mpz_t(), 10);
	return *this;
    }
    digit_iterator operator++(int){
	digit_iterator tmp(*this);
	operator++();
	return tmp;
    }
    bool operator==(digit_iterator const& o) const {
	return current == o.current && source == o.source;
    }
    bool operator!=(digit_iterator const& o) const {
	return !operator==(o);
    }
};

extern digit_iterator const digit_iterator_end;

ulong digit_sum(bigint const& b) noexcept;

template <class Iter>
    ulong from_digits(Iter begin, Iter end){
	ulong s = 0;
	while(begin != end){
	    s *= 10;
	    s += *begin++;
	}
	return s;
    }

ulong gcd(ulong a, ulong b);

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
