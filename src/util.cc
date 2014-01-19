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
bool is_prime(ulong x){
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
	ulong p = *++pi;
	if(p * p > x)
	    return true;
	if(x % p == 0)
	    return false;
    }
}
std::vector<ulong> factors(ulong x){
    if(x < 1)
	throw std::domain_error("factors(0)");
    prime_iterator iter;
    std::vector<ulong> factors;
    while(true){
	ulong p = *iter++;
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
std::vector<std::pair<ulong, unsigned char>> ct_factors(ulong x){
    if(x < 1)
	throw std::domain_error("ct_factors(0)");
    prime_iterator iter;
    decltype(ct_factors(x)) factors;
    while(true){
	ulong p = *iter++;
	if(p * p > x)
	    break;
	if(x % p == 0){
	    ulong ct = 0;
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
uint isqrt(ulong const x){
    uint c = 1 << 31;
    uint g = c;
    while(c != 0){
	g |= c;
	ulong const gsq = ((ulong) g) * g;
	if(gsq > x)
	    g ^= c;
	else if(gsq == x)
	    return g;
	c >>= 1;
    }
    return 0;
}
uint isqrt_part(ulong const x){
    uint c = 1 << 31;
    uint g = c;
    while(c != 0){
	g |= c;
	ulong const gsq = ((ulong) g) * g;
	if(gsq > x)
	    g ^= c;
	c >>= 1;
    }
    return g;
}
ulong divisor_ct(ulong x){
    ulong ct = 1;
    for(auto p : ct_factors(x))
	ct *= (p.second + 1);
    return ct;
}
typedef decltype(divisors(0)) div_ret;
typedef decltype(ct_factors(0)) ct_fact_ret;
void divisors_rec(ct_fact_ret::const_reverse_iterator first, decltype(first) const last,
	ulong val, div_ret& into){
    if(first == last){
	into.push_back(val);
    } else {
	auto next = *first++;
	for(ulong i = 0; i <= next.second; ++i){
	    divisors_rec(first, last, val, into);
	    val *= next.first;
	}
    }
}
div_ret divisors(ulong x){
    div_ret cont;
    auto factors = ct_factors(x);
    divisors_rec(factors.crbegin(), factors.crend(), 1, cont);
    return cont;
}

digit_iterator const digit_iterator_end;

ulong digit_sum(bigint const& b) noexcept {
    ulong sum = 0;
    big_digit_iterator iter(b);
    while(iter != big_digit_iterator()){
	sum += *iter;
	++iter;
    }
    return sum;
}

ulong divisor_sum(ulong x){
    ulong sum = 0;
    for(auto d : divisors(x))
	sum += d;
    return sum;
}
ulong gcd(ulong a, ulong b){
    if(a * b == 0)
	return a | b;
    unsigned int shift;
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
ulong totient(ulong n){
    ulong prev_factor = 0;
    ulong tot = 1;
    for(ulong f : factors(n))
	if(f == prev_factor){
	    tot *= f;
	} else {
	    prev_factor = f;
	    tot *= (f - 1);
	}
    return tot;
}

pythag_iterator::pythag_iterator() : pq(triplet_ref_comp{stor},
	std::vector<uint>{}) {
    stor.emplace_back(3,4,5,0);
    pq.push(0);
}
inline bool pythag_iterator::is_prim(uint ref) const {
    return stor[ref].prim_ref == ref;
}
void pythag_iterator::advance() {
    // mats is 3ct 3 by 3 matrices
    static const std::array<int,27> mats = {{1,-2,2,2,-1,2,2,-2,3,
	1,2,2,2,1,2,2,2,3, -1,2,2,-2,1,2,-2,2,3}};
    uint base_r = pq.top();
    if(is_prim(base_r)){
	triplet base = stor[base_r].t;
	pq.pop(); // have to pop first
	auto matsi = mats.cbegin();
	for(uint i = 0; i < 3; ++i){
	    triplet build;
	    for(uint j = 0; j < 3; ++j){
		int v = 0;
		for(uint k = 0; k < 3; ++k)
		    v += ((int) base[k]) * *matsi++;
		build[j] = (uint) v;
	    }
	    if(build[0] > build[1])
		std::swap(build[0],build[1]);
	    uint ref = stor.size();
	    stor.emplace_back(build,ref);
	    pq.push(ref);
	}
	for(uint i = 0; i < 3; ++i)
	    base[i] += base[i];
	uint ref = stor.size();
	stor.emplace_back(base,base_r); // we can't reuse a primitive
	pq.push(ref);
    } else {
	pq.pop();
	// can update stor in place, since nothing refers to non-primitive
	triplet& upd = stor[base_r].t;
	triplet const& prim = stor[stor[base_r].prim_ref].t;
	for(uint i = 0; i < 3; ++i)
	    upd[i] += prim[i];
	pq.push(base_r); // same underlying number, different ordering
    }
}

}
