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

pythag_iterator::pythag_iterator() : pq(triplet_ref_comp{stor},
	std::vector<u32>{}) {
    stor.emplace_back(3,4,5,0);
    pq.push(0);
}
inline bool pythag_iterator::is_prim(u32 ref) const {
    return stor[ref].prim_ref == ref;
}
void pythag_iterator::advance() {
    // mats is 3ct 3 by 3 matrices
    static const std::array<i32,27> mats = {{1,-2,2,2,-1,2,2,-2,3,
	1,2,2,2,1,2,2,2,3, -1,2,2,-2,1,2,-2,2,3}};
    u32 base_r = pq.top();
    if(is_prim(base_r)){
	triplet base = stor[base_r].t;
	pq.pop(); // have to pop first
	auto matsi = mats.cbegin();
	for(u32 i = 0; i < 3; ++i){
	    triplet build;
	    for(u32 j = 0; j < 3; ++j){
		i32 v = 0;
		for(u32 k = 0; k < 3; ++k)
		    v += ((i32) base[k]) * *matsi++;
		build[j] = (u32) v;
	    }
	    if(build[0] > build[1])
		std::swap(build[0],build[1]);
	    u32 ref = stor.size();
	    stor.emplace_back(build,ref);
	    pq.push(ref);
	}
	for(u32 i = 0; i < 3; ++i)
	    base[i] += base[i];
	u32 ref = stor.size();
	stor.emplace_back(base,base_r); // we can't reuse a primitive
	pq.push(ref);
    } else {
	pq.pop();
	// can update stor in place, since nothing refers to non-primitive
	triplet& upd = stor[base_r].t;
	triplet const& prim = stor[stor[base_r].prim_ref].t;
	for(u32 i = 0; i < 3; ++i)
	    upd[i] += prim[i];
	pq.push(base_r); // same underlying number, different ordering
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

std::array<char const *const, 1786> input_words = {{
#include "words.include"
}};

}
