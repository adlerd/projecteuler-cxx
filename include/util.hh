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

template <uint sides>
    struct figurate_constants {
	static ulong constexpr bsq = 16 + sides*sides - 8*sides;
	static ulong constexpr eight_a = 8 * sides - 16;
	static ulong constexpr two_a = 2 * sides - 4;
	static ulong constexpr mod = (two_a - ((sides - 4) % two_a)) % two_a;
    };
template <uint sides>
    class figurate_iterator {
	ulong f;
	ulong d;
    public:
	figurate_iterator() : f(1), d(sides-1) {}
	explicit figurate_iterator(ulong n)
	    : f(n*((sides-2)*n+4-sides)/2), d(n*(sides-2)+1) {}
	figurate_iterator(figurate_iterator const&) = default;
	figurate_iterator(figurate_iterator&&) = default;
	figurate_iterator<sides>& operator++(){
	    f += d;
	    d += sides - 2;
	    return *this;
	}
	figurate_iterator<sides> operator++(int){
	    figurate_iterator<sides> const tmp(*this);
	    operator++();
	    return tmp;
	}
	ulong operator*() const {
	    return f;
	}
	ulong next_diff() const {
	    return d;
	}
	static figurate_iterator at_least(ulong p){
	    typedef figurate_constants<sides> fcs;
	    ulong const ipart = isqrt_part((p - 1) * fcs::eight_a + fcs::bsq);
	    ulong const numer = ipart + sides - 4;
	    return figurate_iterator(1 + numer/fcs::two_a);
	}
    };
template <uint sides>
    bool is_figurate(ulong p){
	typedef figurate_constants<sides> fcs;
	ulong const sqrt = isqrt(p * fcs::eight_a + fcs::bsq);
	return sqrt != 0 && sqrt % fcs::two_a == fcs::mod;
    }
template <>
    inline bool is_figurate<4>(ulong p){
	return isqrt(p) != 0;
    }
namespace figurate {
    typedef figurate_iterator<3> tri_iter;
    typedef figurate_iterator<4> square_iter;
    typedef figurate_iterator<5> pent_iter;
    typedef figurate_iterator<6> hex_iter;
}

class big_digit_iterator : public std::iterator<std::forward_iterator_tag, unsigned char> {
    bigint source;
    unsigned char current;
public:
    big_digit_iterator() : source(0), current(0) {}
    explicit big_digit_iterator(bigint const& s) : source(s) {
	operator++();
    }
    unsigned char operator*() const {
	return current;
    }
    big_digit_iterator& operator++() noexcept {
	current = mpz_fdiv_q_ui(source.get_mpz_t(), source.get_mpz_t(), 10);
	return *this;
    }
    big_digit_iterator operator++(int){
	big_digit_iterator tmp(*this);
	operator++();
	return tmp;
    }
    bool operator==(big_digit_iterator const& o) const {
	return current == o.current && source == o.source;
    }
    bool operator!=(big_digit_iterator const& o) const {
	return !operator==(o);
    }
};
class digit_iterator : public std::iterator<std::forward_iterator_tag, unsigned char> {
    ulong source;
    unsigned char current;
public:
    digit_iterator() : source(0), current(0) {}
    explicit digit_iterator(ulong const& s) : source(s) {
	operator++();
    }
    unsigned char operator*() const {
	return current;
    }
    digit_iterator& operator++() noexcept {
	current = source % 10;
	source /= 10;
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
public:
    problem(ulong n) : number(n) {}
    virtual ~problem() {}
    ulong get_number() const { return number; }
    virtual std::string run() const = 0;
    bool operator<(problem const& other) const {
	return number < other.number;
    }
};

template <class T>
inline std::string euler_to_string(T const& x){
    return std::to_string(x);
}
template <>
inline std::string euler_to_string<std::string>(std::string const& x){
    return x;
}
template <>
inline std::string euler_to_string<bigint>(bigint const& x){
    return x.get_str();
}


template <class T>
class typed_problem : public problem {
    typedef T (*fun_t)();
    fun_t fun;
public:
    typed_problem(ulong n, fun_t f) : problem(n), fun(f) {}
    virtual std::string run() const {
	return euler_to_string(fun());
    }
};

template <class T>
problem const *new_problem(ulong n, T (*fun)()) {
    return new typed_problem<T>(n, fun);
}

}

#endif
