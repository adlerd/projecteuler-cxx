#include <array>
#include <algorithm>
#include <set>

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
    template <ulong coin_value, class L>
	struct coin_loop {
	    L inner;
	    coin_loop(L&& i)
		: inner(i) {}
	    void operator()(ulong total) const {
		while(total <= 200){
		    inner(total);
		    total += coin_value;
		}
	    }
	};
    std::string problem31(){
	ulong ct = 0;
	auto const lambda = [&](ulong total){++ct;};
	coin_loop<200,coin_loop<100,coin_loop<50,coin_loop<20,coin_loop<10,
	    coin_loop<5,coin_loop<2,decltype(lambda)>>>>>>> const
		cl {{{{{{{std::move(lambda)}}}}}}};
	cl(0);
	return std::to_string(ct);
    }
    template <class Iter>
	ulong from_digits(Iter begin, Iter end){
	    ulong s = 0;
	    while(begin != end){
		s *= 10;
		s += *begin++;
	    }
	    return s;
	}
    std::string problem32(){
	std::array<unsigned char, 9> digits = {{1, 2, 3, 4, 5, 6, 7, 8, 9}};
	std::set<ulong> products;
	do {
	    ulong back = from_digits(digits.cbegin() + 5, digits.cend());
	    for(unsigned int const split : {1,2}){
		ulong p = from_digits(digits.cbegin(), digits.cbegin()+split)
		    * from_digits(digits.cbegin()+split, digits.cbegin() + 5);
		if(p == back)
		    products.insert(p);
	    }
	} while(std::next_permutation(digits.begin(), digits.end()));
	ulong s = 0;
	for(ulong p : products)
	    s += p;
	return std::to_string(s);
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set3
    {{P(30),P(31),P(32)}};
}
