#include "set2.hh"

namespace euler {
    std::string problem20(){
	bigint fac;
	mpz_fac_ui(fac.get_mpz_t(), 100);
	return std::to_string(digit_sum(fac));
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set2
    {{P(20)}};
}
