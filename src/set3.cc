// Copyright 2014 David Adler

#include <array>
#include <algorithm>
#include <set>
#include <bitset>

#include "util.hh"
#include "atkin.hh"

namespace {
    using namespace euler;
    std::array<u64, 10> constexpr five_dig
    {{ 0, 1, 1 << 5, 81 * 3, 1 << 10, 125 * 25, (81 * 3) << 5, 49 * 49 * 7, 1 << 15,
	 81 * 81 * 9 }};
    u64 problem30(){
	u64 sum = 0;
	for(u64 n = 2; n < 200000; ++n){
	    u64 dsum = 0;
	    u64 x = n;
	    while(x > 0 && dsum <= n){
		u64 dig = x % 10;
		x /= 10;
		dsum += five_dig[dig];
	    }
	    if(dsum == n)
		sum += n;
	}
	return sum;
    }
    template <u64 coin_value, class L>
	struct coin_loop {
	    L inner;
	    coin_loop(L&& i)
		: inner(i) {}
	    void operator()(u64 total) const {
		while(total <= 200){
		    inner(total);
		    total += coin_value;
		}
	    }
	};
    u64 problem31(){
	u64 ct = 0;
	auto const lambda = [&](u64 total){++ct;};
	coin_loop<200,coin_loop<100,coin_loop<50,coin_loop<20,coin_loop<10,
	    coin_loop<5,coin_loop<2,decltype(lambda)>>>>>>> const
		cl {{{{{{{std::move(lambda)}}}}}}};
	cl(0);
	return ct;
    }
    u64 problem32(){
	std::array<u8, 9> digits = {{1, 2, 3, 4, 5, 6, 7, 8, 9}};
	std::set<u64> products;
	do {
	    u64 back = from_digits(digits.cbegin() + 5, digits.cend());
	    for(u32 const split : {1,2}){
		u64 p = from_digits(digits.cbegin(), digits.cbegin()+split)
		    * from_digits(digits.cbegin()+split, digits.cbegin() + 5);
		if(p == back)
		    products.insert(p);
	    }
	} while(std::next_permutation(digits.begin(), digits.end()));
	u64 s = 0;
	for(u64 p : products)
	    s += p;
	return s;
    }
    template <class N>
	inline std::pair<N, N> lowest_terms(N n, N d){
	    u64 const g = gcd(n,d);
	    return {n / g, d / g};
	}
    u64 problem33(){
	std::pair<u64,u64> product({1,1});
	for(u8 a = 1; a < 10; ++a)
	    for(u8 b = 1; b < 10; ++b)
		for(u8 c = 1; c < 10; ++c){
		    auto simple = lowest_terms(a,b);
		    for(u8 n : {10*a+c, 10*c+a})
			for(u8 d : {10*b+c, 10*c+b})
			    if(n < d && simple == lowest_terms(n,d))
				product = lowest_terms(product.first * n, product.second * d);
		}
	return product.second;
    }
    std::array<u64, 10> dfact = {{1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880}};
    u64 problem34(){
	u64 sum = 0;
	for(u64 i = 10; i < 50000; ++i){
	    u64 ds = 0;;
	    for(digit_iterator di(i); ds <= i &&  di != digit_iterator(); ++di)
		ds += dfact[*di];
	    if(i == ds)
		sum += i;
	}
	return sum;
    }
    u64 problem35(){
	u64 ct = 0;
	std::deque<u8> rev_digits;
	for(prime_iterator pi; *pi < 1000000; ++pi){
	    rev_digits.assign(digit_iterator(*pi), digit_iterator());
	    u64 const size = rev_digits.size();
	    for(u32 i = 1; i < size; ++i){
		rev_digits.push_front(rev_digits.back());
		rev_digits.pop_back();
		if(!is_prime(from_digits(rev_digits.crbegin(), rev_digits.crend())))
		    goto dead;
	    }
	    ++ct;
	    dead:;
	}
	return ct;
    }
    u64 problem36(){
	u64 sum = 0;
	for(u64 i = 1; i < 1000000; ++i)
	    if(is_palindrome(std::to_string(i))){
		std::string bin;
		for(u64 j = i; j > 0; j >>= 1)
		    bin += j & 1 ? '1' : '0';
		if(is_palindrome(bin))
		    sum += i;
	    }
	return sum;
    }
    u64 problem37(){
	prime_iterator iter;
	u64 ct = 0;
	u64 sum = 0;
	while(*iter < 10) ++iter;
	std::vector<u8> rev_digits;
	while(ct < 11){
	    u64 const p = *iter++;
	    rev_digits.assign(digit_iterator(p), digit_iterator());
	    for(u32 i = 1; i < rev_digits.size(); ++i){
		if(!is_prime(from_digits(rev_digits.crbegin()+i, rev_digits.crend()))
			|| !is_prime(from_digits(rev_digits.crbegin(), rev_digits.crend()-i)))
		    goto bad;
	    }
	    ++ct;
	    sum += p;
	    bad:;
	}
	return sum;
    }
    std::array<u32, 7> ten_lookup{{1, 10, 100, 1000, 10000, 100000, 1000000}};
    u64 problem38(){
	u64 max = 0;
	std::array<u8, 9> concat;
	std::array<u64, 6> starts{{1, 10, 100, 1000, 10000, 100000}};
	for(u32 dct = 1; dct < 7; ++dct){
	    for(u64 i = starts[dct-1]; i < ten_lookup[dct]; ++i){
		std::bitset<9> ds = 0;
		auto at = concat.begin();
		for(u64 m = 1; !ds.all(); ++m){
		    auto beg = at;
		    digit_iterator di(m*i);
		    while(di != digit_iterator()){
			if(at == concat.end())
			    goto bad;
			u8 c = *di++;
			if(c == 0 || ds[c-1])
			    goto bad;
			ds[c-1] = true;
			*at++ = c;
		    }
		    std::reverse(beg, at);
		}
		{
		    u64 n = from_digits(concat.cbegin(), concat.cend());
		    if(n > max){
			max = n;
			for(u32 i = dct; i < 7; ++i)
			    starts[i - i] = from_digits(concat.cbegin(), concat.cbegin() + i);
		    }
		}
bad:;
	    }
	}
	return max;
    }
    u64 problem39(){
	std::vector<u8> arr(500,0);
	for(u64 m = 2; m * m * 2 < 1000; ++m){
	    for(u64 n = m % 2 + 1; n < m; n += 2){
		u64 const sp = m * (m + n);
		if(sp >= 500)
		    break;
		if(gcd(m,n) == 1)
		    for(u64 i = 1; i * sp < 500; ++i)
			++arr[i * sp];
	    }
	}
	auto iter = std::max_element(arr.cbegin(), arr.cend());
	return 2 * (iter - arr.cbegin());
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set3
    {P(30),P(31),P(32),P(33),P(34),P(35),P(36),P(37),P(38),P(39)};
}
