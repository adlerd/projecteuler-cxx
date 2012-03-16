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
    digit_iterator iter(b);
    while(iter != digit_iterator_end){
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
}
