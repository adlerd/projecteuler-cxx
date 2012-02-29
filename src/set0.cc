#include <stdexcept>
#include <utility>
#include <array>

#include "util.hh"
#include "set0.hh"
#include "atkin.hh"

namespace euler {
    std::string problem1(){
	ulong sum = 0;
	for(ulong i = 0; i < 1000; ++i)
	    if((i%3) * (i%5) == 0)
		sum += i;
	return std::to_string(sum);
    }
    std::string problem2(){
	ulong a = 0;
	ulong b = 1;
	ulong sum = 0;
	while(a < 4000000){
	    if(a%2 == 0)
		sum += a;
	    ulong const t = a + b;
	    a = b;
	    b = t;
	}
	return std::to_string(sum);
    }
    std::string problem3(){
	return std::to_string(factors(600851475143).back());
    }
    bool is_palindrome(std::string const& str){
	auto middle = str.cbegin() + str.size() / 2;
	return std::mismatch(str.cbegin(), middle, str.crbegin()).first == middle;
    }
    std::string problem4(){
	ulong high = 0;
	for(ulong i = 999; i < 1000; --i)
	    for(ulong j = 999; j >= i; --j){
		ulong const p = i * j;
		if(p <= high) 
		    break;
		auto str = std::to_string(i*j);
		if(is_palindrome(str))
		    high = p;
	    }
	return std::to_string(high);
    }
    ulong gcd(ulong a, ulong b){
	if(a * b == 0)
	    return a | b;
	unsigned int shift;
	for(shift = 0; ((a|b) & 1) == 0; ++shift){
	    a >>= 1;
	    b >>= 1;
	}
	while((a & 1) == 0)
	    a >>= 1;
	do {
	    while((b & 1) == 0)
		b >>= 1;
	    if(a > b)
		std::swap(a, b);
	    b -= a;
	} while(b != 0);
	return a << shift;
    }
    std::string problem5(){
	ulong g = 1;
	for(ulong i = 2; i <= 20; ++i)
	    g = (g * i) / gcd(g, i);
	return std::to_string(g);
    }
    std::string problem6(){
	ulong sum = 0;
	ulong sum_sq = 0;
	for(ulong i = 1; i <= 100; ++i){
	    sum += i;
	    sum_sq += i * i;
	}
	sum *= sum;
	return std::to_string(sum > sum_sq ? sum - sum_sq : sum_sq - sum);
    }
    std::string problem7(){
	prime_iterator pi;
	for(int i = 0; i < 10000; ++i)
	    ++pi;
	return std::to_string(*pi);
    }
    std::array<unsigned char, 1005> constexpr input8 {{0,0,0,0,0,
	7,3,1,6,7,1,7,6,5,3,1,3,3,0,6,2,4,9,1,9,2,2,5,1,1,9,6,7,4,4,2,6,5,7,4,
	7,4,2,3,5,5,3,4,9,1,9,4,9,3,4,9,6,9,8,3,5,2,0,3,1,2,7,7,4,5,0,6,3,2,6,
	2,3,9,5,7,8,3,1,8,0,1,6,9,8,4,8,0,1,8,6,9,4,7,8,8,5,1,8,4,3,8,5,8,6,1,
	5,6,0,7,8,9,1,1,2,9,4,9,4,9,5,4,5,9,5,0,1,7,3,7,9,5,8,3,3,1,9,5,2,8,5,
	3,2,0,8,8,0,5,5,1,1,1,2,5,4,0,6,9,8,7,4,7,1,5,8,5,2,3,8,6,3,0,5,0,7,1,
	5,6,9,3,2,9,0,9,6,3,2,9,5,2,2,7,4,4,3,0,4,3,5,5,7,6,6,8,9,6,6,4,8,9,5,
	0,4,4,5,2,4,4,5,2,3,1,6,1,7,3,1,8,5,6,4,0,3,0,9,8,7,1,1,1,2,1,7,2,2,3,
	8,3,1,1,3,6,2,2,2,9,8,9,3,4,2,3,3,8,0,3,0,8,1,3,5,3,3,6,2,7,6,6,1,4,2,
	8,2,8,0,6,4,4,4,4,8,6,6,4,5,2,3,8,7,4,9,3,0,3,5,8,9,0,7,2,9,6,2,9,0,4,
	9,1,5,6,0,4,4,0,7,7,2,3,9,0,7,1,3,8,1,0,5,1,5,8,5,9,3,0,7,9,6,0,8,6,6,
	7,0,1,7,2,4,2,7,1,2,1,8,8,3,9,9,8,7,9,7,9,0,8,7,9,2,2,7,4,9,2,1,9,0,1,
	6,9,9,7,2,0,8,8,8,0,9,3,7,7,6,6,5,7,2,7,3,3,3,0,0,1,0,5,3,3,6,7,8,8,1,
	2,2,0,2,3,5,4,2,1,8,0,9,7,5,1,2,5,4,5,4,0,5,9,4,7,5,2,2,4,3,5,2,5,8,4,
	9,0,7,7,1,1,6,7,0,5,5,6,0,1,3,6,0,4,8,3,9,5,8,6,4,4,6,7,0,6,3,2,4,4,1,
	5,7,2,2,1,5,5,3,9,7,5,3,6,9,7,8,1,7,9,7,7,8,4,6,1,7,4,0,6,4,9,5,5,1,4,
	9,2,9,0,8,6,2,5,6,9,3,2,1,9,7,8,4,6,8,6,2,2,4,8,2,8,3,9,7,2,2,4,1,3,7,
	5,6,5,7,0,5,6,0,5,7,4,9,0,2,6,1,4,0,7,9,7,2,9,6,8,6,5,2,4,1,4,5,3,5,1,
	0,0,4,7,4,8,2,1,6,6,3,7,0,4,8,4,4,0,3,1,9,9,8,9,0,0,0,8,8,9,5,2,4,3,4,
	5,0,6,5,8,5,4,1,2,2,7,5,8,8,6,6,6,8,8,1,1,6,4,2,7,1,7,1,4,7,9,9,2,4,4,
	4,2,9,2,8,2,3,0,8,6,3,4,6,5,6,7,4,8,1,3,9,1,9,1,2,3,1,6,2,8,2,4,5,8,6,
	1,7,8,6,6,4,5,8,3,5,9,1,2,4,5,6,6,5,2,9,4,7,6,5,4,5,6,8,2,8,4,8,9,1,2,
	8,8,3,1,4,2,6,0,7,6,9,0,0,4,2,2,4,2,1,9,0,2,2,6,7,1,0,5,5,6,2,6,3,2,1,
	1,1,1,1,0,9,3,7,0,5,4,4,2,1,7,5,0,6,9,4,1,6,5,8,9,6,0,4,0,8,0,7,1,9,8,
	4,0,3,8,5,0,9,6,2,4,5,5,4,4,4,3,6,2,9,8,1,2,3,0,9,8,7,8,7,9,9,2,7,2,4,
	4,2,8,4,9,0,9,1,8,8,8,4,5,8,0,1,5,6,1,6,6,0,9,7,9,1,9,1,3,3,8,7,5,4,9,
	9,2,0,0,5,2,4,0,6,3,6,8,9,9,1,2,5,6,0,7,1,7,6,0,6,0,5,8,8,6,1,1,6,4,6,
	7,1,0,9,4,0,5,0,7,7,5,4,1,0,0,2,2,5,6,9,8,3,1,5,5,2,0,0,0,5,5,9,3,5,7,
	2,9,7,2,5,7,1,6,3,6,2,6,9,5,6,1,8,8,2,6,7,0,4,2,8,2,5,2,4,8,3,6,0,0,8,
	2,3,2,5,7,5,3,0,4,2,0,7,5,2,9,6,3,4,5,0}};
    std::string problem8(){
	ulong zct = 5;
	auto left = input8.cbegin();
	auto right = left + 5;
	auto const end = input8.cend();
	ulong prod = 1;
	ulong high = 0;
	while(right != end){
	    ulong const p = *right++;
	    ulong const d = *left++;
	    if(d != 0)
		prod /= d;
	    else
		--zct;
	    if(p != 0)
		prod *= p;
	    else
		++zct;
	    if(zct == 0 && prod > high)
		high = prod;
	}
	return std::to_string(high);
    }
    std::string problem9(){
	for(ulong b = 1; b < 1000; ++b){
	    ulong const lim = b > 500 ? 1000 - b : b;
	    for(ulong a = 1; a < lim; ++a){
		ulong const c = 1000 - b - a;
		if(a*a + b*b == c*c)
		    return std::to_string(a*b*c);
	    }
	}
	throw std::logic_error("problem 9 unsolved");
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set0
    {{P(1), P(2), P(3), P(4), P(5), P(6), P(7), P(8), P(9)}};
}