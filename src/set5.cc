#include "set5.hh"
#include "util.hh"
#include "atkin.hh"

namespace euler {
    std::string problem50(){
	ulong max_ct = 0;
	ulong max_prime = 0;
	for(prime_iterator start; *start < 1000; ++start){
	    ulong sum = 0;
	    ulong ct = 0;
	    for(prime_iterator end(start); sum < 1000000; ++end){
		if(ct > max_ct && is_prime(sum)){
		    max_ct = ct;
		    max_prime = sum;
		}
		++ct;
		sum += *end;
	    }
	}
	return std::to_string(max_prime);
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set5
    {{P(50)}};
}
