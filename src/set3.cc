#include <array>

#include "set3.hh"
#include "util.hh"

namespace euler {
    std::array<ulong, 10> constexpr five_dig
    {{ 0, 1, 1 << 5, 81 * 3, 1 << 10, 125 * 25, (81 * 3) << 5, 49 * 49 * 7, 1 << 15,
	 81 * 81 * 9 }};
    std::string problem30(){
	ulong sum = 0;
	for(ulong n = 2; n < 200000; ++n){
	    ulong dsum = 0;
	    ulong x = n;
	    while(x > 0 && dsum <= n){
		ulong dig = x % 10;
		x /= 10;
		dsum += five_dig[dig];
	    }
	    if(dsum == n)
		sum += n;
	}
	return std::to_string(sum);
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set3
    {{P(30)}};
}
