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
}
