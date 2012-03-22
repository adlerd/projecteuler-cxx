#include <array>
#include <algorithm>

#include "set4.hh"
#include "util.hh"

namespace euler {
    std::string problem40(){
	ulong p = 1;
	std::vector<uchar> rdigs = {0};
	auto iter = rdigs.rbegin();
	ulong n = 0;
	ulong i = 0;
	ulong target = 1;
	while(target <= 1000000){
	    while(i < target){
		if(++iter == rdigs.rend()){
		    rdigs.assign(digit_iterator(++n), digit_iterator());
		    iter = rdigs.rbegin();
		}
		++i;
	    }
	    p *= *iter;
	    target *= 10;
	}
	return std::to_string(p);
    }
    std::string problem41(){
	std::array<uchar, 9> digs = {{ 9, 8, 7, 6, 5, 4, 3, 2, 1 }};
	ulong r_ct = 0;
	while(true){
	    if(!std::prev_permutation(digs.begin() + r_ct, digs.end()))
		++r_ct;
	    ulong n = from_digits(digs.begin() + r_ct, digs.end());
	    if(is_prime(n))
		return std::to_string(n);
	}
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set4
    {{P(40),P(41)}};
}
