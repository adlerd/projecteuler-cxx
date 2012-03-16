#include <array>
#include <algorithm>
#include <set>

#include "set3.hh"
#include "util.hh"
#include "atkin.hh"

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
    template <class N>
	inline std::pair<N, N> lowest_terms(N n, N d){
	    ulong const g = gcd(n,d);
	    return {n / g, d / g};
	}
    std::string problem33(){
	std::pair<ulong,ulong> product({1,1});
	for(unsigned char a = 1; a < 10; ++a)
	    for(unsigned char b = 1; b < 10; ++b)
		for(unsigned char c = 1; c < 10; ++c){
		    auto simple = lowest_terms(a,b);
		    for(unsigned char n : {10*a+c, 10*c+a})
			for(unsigned char d : {10*b+c, 10*c+b})
			    if(n < d && simple == lowest_terms(n,d))
				product = lowest_terms(product.first * n, product.second * d);
		}
	return std::to_string(product.second);
    }
    std::array<ulong, 10> dfact = {{1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880}};
    std::string problem34(){
	ulong sum = 0;
	for(ulong i = 10; i < 50000; ++i){
	    ulong ds = 0;;
	    for(digit_iterator di(i); ds <= i &&  di != digit_iterator_end; ++di)
		ds += dfact[*di];
	    if(i == ds)
		sum += i;
	}
	return std::to_string(sum);
    }
    std::string problem35(){
	ulong ct = 0;
	std::deque<unsigned char> rev_digits;
	for(prime_iterator pi; *pi < 1000000; ++pi){
	    rev_digits.assign(digit_iterator(*pi), digit_iterator());
	    ulong const size = rev_digits.size();
	    for(unsigned int i = 1; i < size; ++i){
		rev_digits.push_front(rev_digits.back());
		rev_digits.pop_back();
		if(!is_prime(from_digits(rev_digits.crbegin(), rev_digits.crend())))
		    goto dead;
	    }
	    ++ct;
	    dead:;
	}
	return std::to_string(ct);
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set3
    {{P(30),P(31),P(32),P(33),P(34),P(35)}};
}
