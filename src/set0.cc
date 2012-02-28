#include <stdexcept>

#include "util.hh"
#include "set0.hh"

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
#define P(x) {x, &problem ## x}
    std::list<problem> set0
    {{P(1), P(2), P(3), P(4)}};
}
