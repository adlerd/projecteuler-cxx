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
typedef uint8_t uchar;
typedef uint32_t uint;
typedef uint64_t ulong;
typedef mpz_class bigint;
typedef std::string (*problem_fun)();
typedef std::pair<uint, std::string> answer;

extern problem_fun NYI_problem;

bool is_palindrome(std::string const& str);

bool is_prime(ulong x);
std::vector<ulong> factors(ulong x);
std::vector<std::pair<ulong, unsigned char>> ct_factors(ulong x);

/* isqrt and isqrt_part return the square root of perfect squares. for
 * non-squares, isqrt returns 0 and isqrt_part returns floor(sqrt) */
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

ulong totient(ulong n);

class problem {
    uint number;
public:
    problem(uint n) : number(n) {}
    virtual ~problem() {}
    uint get_number() const { return number; }
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

class pythag_iterator {
public:
    typedef std::array<uint,3> triplet;
private:
    struct triplet_data {
	triplet t;
	uint prim_ref;
	triplet_data(triplet const& tt, uint pr) : t(tt), prim_ref(pr) {}
	triplet_data(uint a, uint b, uint c, uint pr)
	    : t{{a,b,c}}, prim_ref(pr) {}
    };
    typedef std::vector<triplet_data> stor_t;
    struct triplet_ref_comp {
	stor_t const& stor;
	bool operator()(uint b, uint a) const { // intentionally switched a,b
	    return std::lexicographical_compare(stor[a].t.cbegin(),
		    stor[a].t.cend(), stor[b].t.cbegin(), stor[b].t.cend());
	}
    };
    stor_t stor;
    std::priority_queue<uint,std::vector<uint>,triplet_ref_comp const> pq;
    void advance();
    bool is_prim(uint ref) const;
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

ulong triangle_collapse(uchar const *start, ulong width);
template <class Iter>
ulong high_subseq_prod(Iter first, Iter const last, ulong len){
    ulong zct = 0;
    ulong prod = 1;
    typename std::remove_reference<Iter>::type middle(first);
    for(ulong i = 0; i < len; ++i){
	if(middle == last)
	    return 0;
	ulong const p = *middle++;
	if(p == 0)
	    ++zct;
	else
	    prod *= p;
    }
    assert(middle != first);
    ulong high = zct == 0 ? prod : 0;
    while(middle != last){
	ulong const p = *middle++;
	ulong const d = *first++;
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

extern std::list<std::list<problem const*> const*> const sets;

}

#endif
