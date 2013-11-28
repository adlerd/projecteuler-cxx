#include "set8.hh"
#include "util.hh"
#include <cassert>

namespace euler {
    uint problem80(){
	uint psr = 2;
	uint psq = 4;
	uint sum = 0;
	bigint mult;
	mpz_ui_pow_ui(mult.get_mpz_t(), 100, 100);
	for(uint i = 2; i < 100; ++i){
	    if(i == psq){
		psr++;
		psq = psr*psr;
	    } else {
		bigint x = mult * i;
		x = sqrt(x);
		auto str = x.get_str();
		assert(str.size() >= 100);
		for(uint k = 0; k < 100; ++k)
		    sum += str[k] - '0';
	    }
	}
	return sum;
    }
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set8
    {{P(80)}};
}
