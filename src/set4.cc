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
#define P(x) {x, &problem ## x}
    std::list<problem> set4
    {{P(40)}};
}
