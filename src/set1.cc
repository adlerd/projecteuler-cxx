#include "atkin.hh"
#include "util.hh"
#include "set1.hh"

namespace euler {
    std::string problem10(){
	prime_iterator pi;
	ulong sum = 0;
	while(*pi < 2000000){
	    ulong p = *pi++;
	    sum += p;
	}
	return std::to_string(sum);
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set1
    {{P(10)}};
}
