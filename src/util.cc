// Copyright 2014 David Adler

#include <stdexcept>

#include "util.hh"
#include "atkin.hh"

namespace euler {
bool is_palindrome(std::string const& str){
    auto middle = str.cbegin() + str.size() / 2;
    return std::mismatch(str.cbegin(), middle, str.crbegin()).first == middle;
}
std::string NYI_fun(){
    return "NYI";
}
problem_fun NYI_problem = &NYI_fun;
bool is_prime(u64 x){
    switch(x){
    case 0:
    case 1:
	return false;
    case 2:
	return true;
    }
    if(x % 2 == 0) //common case fast
	return false;
    prime_iterator pi;
    while(true){
	u64 p = *++pi;
	if(p * p > x)
	    return true;
	if(x % p == 0)
	    return false;
    }
}
std::vector<u64> factors(u64 x){
    if(x < 1)
	throw std::domain_error("factors(0)");
    prime_iterator iter;
    std::vector<u64> factors;
    while(true){
	u64 p = *iter++;
	if(p * p > x)
	    break;
	while(x % p == 0){
	    factors.push_back(p);
	    x /= p;
	}
    }
    if(x != 1)
	factors.push_back(x);
    return factors;
}
std::vector<std::pair<u64, u8>> ct_factors(u64 x){
    if(x < 1)
	throw std::domain_error("ct_factors(0)");
    prime_iterator iter;
    decltype(ct_factors(x)) factors;
    while(true){
	u64 p = *iter++;
	if(p * p > x)
	    break;
	if(x % p == 0){
	    u64 ct = 0;
	    do {
		++ct;
		x /= p;
	    } while(x % p == 0);
	    factors.push_back({p,ct});
	}
    }
    if(x != 1)
	factors.push_back({x,1});
    return factors;
}
u32 isqrt(u64 const x){
    u32 c = 1 << 31;
    u32 g = c;
    while(c != 0){
	g |= c;
	u64 const gsq = ((u64) g) * g;
	if(gsq > x)
	    g ^= c;
	else if(gsq == x)
	    return g;
	c >>= 1;
    }
    return 0;
}
u32 isqrt_part(u64 const x){
    u32 c = 1 << 31;
    u32 g = c;
    while(c != 0){
	g |= c;
	u64 const gsq = ((u64) g) * g;
	if(gsq > x)
	    g ^= c;
	c >>= 1;
    }
    return g;
}
u64 divisor_ct(u64 x){
    u64 ct = 1;
    for(auto p : ct_factors(x))
	ct *= (p.second + 1);
    return ct;
}
typedef decltype(divisors(0)) div_ret;
typedef decltype(ct_factors(0)) ct_fact_ret;
void divisors_rec(ct_fact_ret::const_reverse_iterator first, decltype(first) const last,
	u64 val, div_ret& into){
    if(first == last){
	into.push_back(val);
    } else {
	auto next = *first++;
	for(u64 i = 0; i <= next.second; ++i){
	    divisors_rec(first, last, val, into);
	    val *= next.first;
	}
    }
}
div_ret divisors(u64 x){
    div_ret cont;
    auto factors = ct_factors(x);
    divisors_rec(factors.crbegin(), factors.crend(), 1, cont);
    return cont;
}

digit_iterator const digit_iterator_end;

u64 digit_sum(bigint const& b) noexcept {
    u64 sum = 0;
    big_digit_iterator iter(b);
    while(iter != big_digit_iterator()){
	sum += *iter;
	++iter;
    }
    return sum;
}

u64 divisor_sum(u64 x){
    u64 sum = 0;
    for(auto d : divisors(x))
	sum += d;
    return sum;
}
u64 gcd(u64 a, u64 b){
    if(a * b == 0)
	return a | b;
    u32 shift;
    for(shift = 0; ((a|b) & 1) == 0; ++shift){
	a >>= 1;
	b >>= 1;
    }
    while((a & 1) == 0)
	a >>= 1;
    do {
	while((b & 1) == 0)
	    b >>= 1;
	if(a > b)
	    std::swap(a, b);
	b -= a;
    } while(b != 0);
    return a << shift;
}
u64 totient(u64 n){
    u64 prev_factor = 0;
    u64 tot = 1;
    for(u64 f : factors(n))
	if(f == prev_factor){
	    tot *= f;
	} else {
	    prev_factor = f;
	    tot *= (f - 1);
	}
    return tot;
}

pythag_iterator::pythag_iterator(bool _nonprim, order o) :
    nonprim(_nonprim), pq(triplet_ref_comp{stor,select_comp(o)},
	    std::vector<u64>{}) {
    stor.emplace_back(3,4,5,1);
    pq.push(0);
}
inline bool pythag_iterator::is_prim(size_t ref) const {
    return !nonprim || stor[ref].mult == 1;
}
void pythag_iterator::advance() {
    // mats is 3ct 3 by 3 matrices
    static const std::array<i64,27> mats = {{1,-2,2,2,-1,2,2,-2,3,
	1,2,2,2,1,2,2,2,3, -1,2,2,-2,1,2,-2,2,3}};
    size_t base_r = pq.top();
    triplet_data base = stor[base_r];
    pq.pop(); // have to pop first
    if(is_prim(base_r)){
	auto matsi = mats.cbegin();
	for(u32 i = 0; i < 3; ++i){
	    triplet build;
	    for(u32 j = 0; j < 3; ++j){
		i64 v = 0;
		for(u32 k = 0; k < 3; ++k)
		    v += ((i64) base.t[k]) * *matsi++;
		build[j] = (u64) v;
	    }
	    if(build[0] > build[1])
		std::swap(build[0],build[1]);
	    size_t ref = stor.size();
	    stor.emplace_back(build,1);
	    pq.push(ref);
	}
    }
    if(nonprim){
	for(u32 i = 0; i < 3; ++i)
	    base.t[i] += base.t[i]/base.mult;
	++base.mult;
	stor[base_r] = base;
	pq.push(base_r);
    }
}

namespace {
    namespace tri_collapse {
	typedef std::vector<u64> ul_vec;
	typedef ul_vec::iterator iterator;
	void collapse_row(iterator const upper_row, iterator const next_row){
	    iterator out = upper_row;
	    u64 prev = *next_row;
	    iterator next = next_row + 1;
	    while(out != next_row){
		u64 here = *next++;
		*out++ += std::max(prev, here);
		prev = here;
	    }
	}
    }
}
u64 triangle_collapse(u8 const *end, u64 width){
    using namespace tri_collapse;
    ul_vec vec(end - (width*(width+1))/2, end);
    iterator mark = vec.end() - width;
    while(--width > 0){
	iterator next = mark - width;
	collapse_row(next, mark);
	mark = next;
    }
    assert(mark == vec.begin());
    return vec.front();
}

namespace euler108 {
    bigint smallestN(u64 target){
	target *= 2;
	target -= 1;
	auto fs = factors(target);
	std::reverse(fs.begin(), fs.end());
	prime_iterator pi;
	bigint ret = 1;
	bigint tmp;
	for(u64 f : fs){
	    mpz_ui_pow_ui(tmp.get_mpz_t(), *pi++, f/2);
	    ret *= tmp;
	}
	return ret;
    }
}
namespace euler129 {
    bool isRepunit(bigint n){
	if(n == 0)
	    return false;
	n *= 9;
	n += 1;
	if(n % 3 == 0 || n % 7 == 0)
	    return false;
	while(n > 0){
	    if(n == 1)
		return true;
	    if(n % 10 != 0)
		return false;
	    n /= 10;
	}
	return true;
    }
    bool badcofactor(){
	throw std::logic_error("did not find next repunit cofactor digit");
    }
    u32 construct_lowrepunit(u64 const n){
	// invariant: (cofactor * n) = R(k)+high_digits*10^k for some
	// cofactor < 10^k, where R(0) = 0
	u32 k = 0;
	u64 high_digits = 0; // cofactor * n  / 10^k
	u8 inverse_mod_10;
	switch(n % 10){
	    case 1: inverse_mod_10 = 1; break;
	    case 3: inverse_mod_10 = 7; break;
	    case 7: inverse_mod_10 = 3; break;
	    case 9: inverse_mod_10 = 9; break;
	    default: badcofactor();
	}
	while(true){
	    // we need high_digits + d*n = 1 mod 10
	    // so d=(1-high_digits)/n mod 10
	    u8 d = ((11 - (high_digits % 10)) * inverse_mod_10) % 10;
	    u64 next_hd = high_digits + d*n;
	    assert(next_hd % 10 == 1);
	    ++k;
	    if(next_hd == 1)
		return k;
	    high_digits = next_hd / 10;
	}
    }
    u32 factor_lowrepunit(u64 const n, std::unordered_map<u32,u32>& pmap){
	u32 result = (n % 3 == 0) ? 3 : 1;
	result *= n;
	u32 lcm = 1;
	for(auto p : ct_factors(n)){
	    u64 const factor = p.first;
	    result /= factor;
	    u32& k = pmap[factor];
	    if(k == 0)
		k = construct_lowrepunit(factor);
	    lcm *= k / gcd(lcm, k);
	}
	lcm *= result / gcd(lcm, result);
	return lcm;
    }
}

u64 gosper(u8 n, u8 k, u64 prev){
    u64 const a = prev & -prev; // lowest set bit of prev
    u64 const b = a + prev; // lowest unset bit left of a
    u64 const c = (prev^b)/(a << 2) | b; // magic
    if(c < (1 << n))
	return c;
    else
	return (1 << k) - 1;
}

std::array<char const *const, 1786> input_words = {{
#include "words.include"
}};

}
