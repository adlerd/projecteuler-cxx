#include "util.hh"

namespace {
    using namespace euler;
    bigint problem100(){
	u64 a0 = 1;
	u64 a1 = 4;
	u64 a2 = 21;
	while(true){
	    // 7*a(n-1)-7*a(n-2)+a(n-3)
	    u64 an = 7*(a2 - a1) + a0;
	    if(an > 1000000000000){
		bigint v = an;
		v = v*(v-1) * 2 + 1;
		mpz_sqrt(v.get_mpz_t(),v.get_mpz_t());
		return (v + 1) / 2;
	    }
	    a0 = a1;
	    a1 = a2;
	    a2 = an;
	}
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set10
    {{P(100)}};
}
