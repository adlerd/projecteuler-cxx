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
}
