#include <array>
#include <vector>
#include <algorithm>

#include "set2.hh"

namespace euler {
    std::string problem20(){
	bigint fac;
	mpz_fac_ui(fac.get_mpz_t(), 100);
	return std::to_string(digit_sum(fac));
    }
    std::string problem21(){
	ulong constexpr limit = 10000;
	std::vector<ulong> vec(1);
	for(ulong i = 1; i < limit; ++i)
	    vec.push_back(divisor_sum(i) - i);
	ulong sum = 0;
	for(ulong i = 1; i < limit; ++i){
	    ulong const ds = vec[i];
	    if(ds != i && (ds < limit ? vec[ds] == i : divisor_sum(ds) == i))
		sum += i;
	}
	return std::to_string(sum);
    }
    std::array<char const *const, 5163> input22 {{
#include "names.include"
    }};
    std::string problem22(){
	ulong sum = 0;
	ulong pos = 0;
	std::vector<std::string> names(input22.cbegin(), input22.cend());
	std::sort(names.begin(), names.end());
	for(auto str : names){
	    ulong psum = 0;
	    for(char ch : str)
		psum += ch + 1 - 'A';
	    sum += psum * ++pos;
	}
	return std::to_string(sum);
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set2
    {{P(20), P(21), P(22)}};
}
