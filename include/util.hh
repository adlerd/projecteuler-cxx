// Copyright 2014 David Adler

#ifndef _UTIL_HH_
#define _UTIL_HH_ 

#include <string>
#include <queue>
#include <list>
#include <utility>
#include <stdint.h>
#include <array>
#include <cassert>

#include <gmpxx.h>

namespace euler {
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef mpz_class bigint;
typedef std::string (*problem_fun)();

extern problem_fun NYI_problem;

bool is_palindrome(std::string const& str);

bool is_prime(u64 x);
std::vector<u64> factors(u64 x);
std::vector<std::pair<u64, u8>> ct_factors(u64 x);

/* isqrt and isqrt_part return the square root of perfect squares. for
 * non-squares, isqrt returns 0 and isqrt_part returns floor(sqrt) */
u32 isqrt(u64 x);
u32 isqrt_part(u64 x);

u64 divisor_ct(u64 x);
std::vector<u64> divisors(u64 x);
u64 divisor_sum(u64 x);

template <u32 sides>
    struct figurate_constants {
	static u64 constexpr bsq = 16 + sides*sides - 8*sides;
	static u64 constexpr eight_a = 8 * sides - 16;
	static u64 constexpr two_a = 2 * sides - 4;
	static u64 constexpr mod = (two_a - ((sides - 4) % two_a)) % two_a;
    };
template <u32 sides>
    class figurate_iterator {
	u64 f;
	u64 d;
    public:
	figurate_iterator() : f(1), d(sides-1) {}
	explicit figurate_iterator(u64 n)
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
	u64 operator*() const {
	    return f;
	}
	u64 next_diff() const {
	    return d;
	}
	static figurate_iterator at_least(u64 p){
	    typedef figurate_constants<sides> fcs;
	    u64 const ipart = isqrt_part((p - 1) * fcs::eight_a + fcs::bsq);
	    u64 const numer = ipart + sides - 4;
	    return figurate_iterator(1 + numer/fcs::two_a);
	}
    };
template <u32 sides>
    bool is_figurate(u64 p){
	typedef figurate_constants<sides> fcs;
	u64 const sqrt = isqrt(p * fcs::eight_a + fcs::bsq);
	return sqrt != 0 && sqrt % fcs::two_a == fcs::mod;
    }
template <>
    inline bool is_figurate<4>(u64 p){
	return isqrt(p) != 0;
    }
namespace figurate {
    typedef figurate_iterator<3> tri_iter;
    typedef figurate_iterator<4> square_iter;
    typedef figurate_iterator<5> pent_iter;
    typedef figurate_iterator<6> hex_iter;
}

class big_digit_iterator : public std::iterator<std::forward_iterator_tag, u8> {
    bigint source;
    u8 current;
public:
    big_digit_iterator() : source(0), current(0) {}
    explicit big_digit_iterator(bigint const& s) : source(s) {
	operator++();
    }
    u8 operator*() const {
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
class digit_iterator : public std::iterator<std::forward_iterator_tag, u8> {
    u64 source;
    u8 current;
public:
    digit_iterator() : source(0), current(0) {}
    explicit digit_iterator(u64 const& s) : source(s) {
	operator++();
    }
    u8 operator*() const {
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

u64 digit_sum(bigint const& b) noexcept;

template <class Iter>
    u64 from_digits(Iter begin, Iter end){
	u64 s = 0;
	while(begin != end){
	    s *= 10;
	    s += *begin++;
	}
	return s;
    }

u64 gcd(u64 a, u64 b);

u64 totient(u64 n);

class problem {
    u32 number;
public:
    problem(u32 n) : number(n) {}
    virtual ~problem() {}
    u32 get_number() const { return number; }
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
    typed_problem(u64 n, fun_t f) : problem(n), fun(f) {}
    virtual std::string run() const {
	return euler_to_string(fun());
    }
};

template <class T>
problem const *new_problem(u64 n, T (*fun)()) {
    return new typed_problem<T>(n, fun);
}

class pythag_iterator {
public:
    typedef std::array<u32,3> triplet;
private:
    struct triplet_data {
	triplet t;
	u32 prim_ref;
	triplet_data(triplet const& tt, u32 pr) : t(tt), prim_ref(pr) {}
	triplet_data(u32 a, u32 b, u32 c, u32 pr)
	    : t{{a,b,c}}, prim_ref(pr) {}
    };
    typedef std::vector<triplet_data> stor_t;
    struct triplet_ref_comp {
	stor_t const& stor;
	bool operator()(u32 b, u32 a) const { // intentionally switched a,b
	    return std::lexicographical_compare(stor[a].t.cbegin(),
		    stor[a].t.cend(), stor[b].t.cbegin(), stor[b].t.cend());
	}
    };
    stor_t stor;
    std::priority_queue<u32,std::vector<u32>,triplet_ref_comp const> pq;
    void advance();
    bool is_prim(u32 ref) const;
public:
    pythag_iterator();
    pythag_iterator& operator++(){
	advance();
	return *this;
    }
    triplet operator*() const {
	return stor[pq.top()].t;
    }
};

namespace util_impl {
/* efficiently finds the rightmost non-maximal element in [first,middle); i.e.,
 * the element to the left of the leftmost element in [first,middle) which is
 * greater than all elements in [middle,end).
 * also returns the element in [middle,end) which is greater than this element.
 * If there is no such element, returns {middle,end}
 */
template <class BiIter>
std::pair<BiIter,BiIter> rightmost_non_maximal(BiIter first, BiIter middle,
	BiIter end){
    if(middle == end)
	return {middle,end};
    BiIter rnm = first;
    BiIter right = middle;
    BiIter last_right = end;
    while(rnm != middle){
	while(!(*rnm < *right)){
	    if(++right == end){
		if(last_right == end)
		    return {middle,end};
		else
		    return {--rnm,last_right};
	    }
	}
	last_right = right;
	++rnm;
    }
    if(last_right == end)
	return {middle,end};
    else
	return {--rnm,last_right};
}
template <class BiIter>
bool shorter(BiIter first1, BiIter last1, BiIter first2, BiIter last2,
	std::bidirectional_iterator_tag){
    while(true){
	if(first2 == last2)
	    return false;
	if(first1 == last1)
	    return true;
	++first1;
	++first2;
    }
}
template <class RAIter>
bool shorter(RAIter first1, RAIter last1, RAIter first2, RAIter last2,
	std::random_access_iterator_tag){
    return (last1 - first1) < (last2 - first2);
}
template <class BiIter>
inline bool shorter(BiIter first1, BiIter last1, BiIter first2, BiIter last2){
    return shorter(first1,last1,first2,last2,typename
	    std::iterator_traits<BiIter>::iterator_category());
}
}
/* pre: [first,middle) is sorted, [middle,end) is sorted
 * post: [first,middle) is the next r-combination, [middle,end) is sorted
 */
template <class BiIter>
bool next_rcombination(BiIter first, BiIter middle, BiIter end){
    using namespace util_impl;
    typedef std::reverse_iterator<BiIter> RevIter;
    std::pair<BiIter,BiIter> rnm_p = rightmost_non_maximal(first,middle,end);
    if(rnm_p.first == middle){
	std::rotate(first,middle,end);
	return false;
    } else {
	if(shorter(rnm_p.first, middle, rnm_p.second, end)){
	    BiIter next = std::swap_ranges(rnm_p.first,middle,rnm_p.second);
	    std::rotate(++rnm_p.second, next, end);
	} else {
	    std::iter_swap(rnm_p.first,rnm_p.second);
	    RevIter r1(end);
	    RevIter r2(++rnm_p.second);
	    RevIter r3(middle);
	    RevIter r4 = std::swap_ranges(r1,r2,r3);
	    std::rotate(++rnm_p.first, r4.base(), middle);
	}
	return true;
    }
}

u64 triangle_collapse(u8 const *start, u64 width);
template <class Iter>
u64 high_subseq_prod(Iter first, Iter const last, u64 len){
    u64 zct = 0;
    u64 prod = 1;
    typename std::remove_reference<Iter>::type middle(first);
    for(u64 i = 0; i < len; ++i){
	if(middle == last)
	    return 0;
	u64 const p = *middle++;
	if(p == 0)
	    ++zct;
	else
	    prod *= p;
    }
    assert(middle != first);
    u64 high = zct == 0 ? prod : 0;
    while(middle != last){
	u64 const p = *middle++;
	u64 const d = *first++;
	if(d == 0)
	    --zct;
	else
	    prod /= d;
	if(p == 0)
	    ++zct;
	else
	    prod *= p;
	if(zct == 0 && prod > high)
	    high = prod;
    }
    return high;
}

namespace euler108 {
    bigint smallestN(u64 target);
}

extern std::array<char const *const, 1786> input_words;

extern std::list<std::list<problem const*> const*> const sets;

}

#endif
