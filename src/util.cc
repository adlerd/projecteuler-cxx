#include <stdexcept>

#include "util.hh"
#include "atkin.hh"

namespace euler {
std::string NYI_fun(){
    return "NYI";
}
problem_fun NYI_problem = &NYI_fun;
std::vector<ulong> factors(ulong x){
    prime_iterator iter;
    if(x < 1)
	throw std::domain_error("factors(0)");
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
    prime_iterator iter;
    if(x < 1)
	throw std::domain_error("factors(0)");
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

ulong digit_sum(bigint b){
    ulong sum = 0;
    while(b > 0)
	sum += mpz_fdiv_q_ui(b.get_mpz_t(), b.get_mpz_t(), 10);
    return sum;
}

ulong divisor_sum(ulong x){
    ulong sum = 0;
    for(auto d : divisors(x))
	sum += d;
    return sum;
}
}
