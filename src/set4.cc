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
    std::string problem43(){
	// use 1, 0, 2... to skip 0, 1, 2 through 0, 9, 8
	std::array<uchar, 10> digs = {{1, 0, 2, 3, 4, 5, 6, 7, 8, 9}};
	ulong sum = 0;
	do {
	    ulong rsum;
	    if(digs[3] % 2 != 0)
		goto next;
	    if((digs[2] + digs[3] + digs[4]) % 3 != 0)
		goto next;
	    if(digs[5] % 5 != 0)
		goto next;
	    rsum = digs[4] * 100 + digs[5] * 10 + digs[6];
	    if(rsum % 7 != 0)
		goto next;
	    rsum = (rsum % 100) * 10 + digs[7];
	    if(rsum % 11 != 0)
		goto next;
	    rsum = (rsum % 100) * 10 + digs[8];
	    if(rsum % 13 != 0)
		goto next;
	    rsum = (rsum % 100) * 10 + digs[9];
	    if(rsum % 17 != 0)
		goto next;
	    sum += from_digits(digs.cbegin(), digs.cend());
next:;
	} while(std::next_permutation(digs.begin(), digs.end()));
	return std::to_string(sum);
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set4
    {{P(40),P(41),P(43)}};
}
