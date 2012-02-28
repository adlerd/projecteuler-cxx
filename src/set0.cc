#include <stdexcept>
#include <utility>

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
#define P(x) {x, &problem ## x}
    std::list<problem> set0
    {{P(1), P(2), P(3), P(4), P(5), P(6), P(7)}};
}
