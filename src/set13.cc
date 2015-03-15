// Copyright 2014 David Adler

#include "atkin.hh"
#include "util.hh"

namespace {
    using namespace euler;
    u32 problem130(){ 
	using namespace euler129;
	std::unordered_map<u32,u32> pmap;
	prime_iterator pi;
	u64 n = 3;
	u64 sum = 0;
	u8 constexpr target = 25;
	u8 ct = 0;
	while(true){
	    ++pi;
	    while(n < *pi){
		if(0 == (n-1)%factor_lowrepunit(n, pmap)){
		    sum += n;
		    if(target <= ++ct)
			return sum;
		}
		n += n%10 == 3 ? 4 : 2;
	    }
	    n += n%10 == 3 ? 4 : 2;
	}
    }
    u32 problem131(){
	// a bit of hand-waving and/or math shows that the primes in question
	// are just the prime differences of adjacent cubes
	u32 constexpr target = 1000000;
	u32 prevCube = 1;
	u32 cube = 8;
	u32 src = 2;
	u32 diff = 7;
	u32 ct = 0;
	prime_iterator pi;
	while(true){
	    while(*pi < diff)
		++pi;
	    if(*pi > target)
		return ct;
	    if(*pi == diff){
		++ct;
		++pi;
	    }
	    while(diff < *pi){
		prevCube = cube;
		++src;
		cube = src*src*src;
		diff = cube - prevCube;
	    }
	}
    }
    u32 problem132(){
	using namespace euler129;
	// given gcd(n,10)==1, n | R(k) iff A(n)|k. probably.
	prime_iterator pi;
	++pi; // skip 2/3
	++pi; // skip 3/5
	u32 sum = 0;
	u64 constexpr k = 1000000000;
	u8 constexpr target = 40;
	u8 ct = 0;
	while(true){
	    u32 prime = *++pi;
	    if(k % construct_lowrepunit(prime) == 0){
		sum += prime;
		if(target <= ++ct)
		    return sum;
	    }
	}
    }
    u64 problem133(){
	using namespace euler129;
	// given gcd(n,10)==1, n | R(k) iff A(n)|k
	// then we want to find n with A(n) having factors other than 2,5
	prime_iterator pi;
	++pi; // skip 2/3
	++pi; // skip 3/5
	u64 sum = 10; // 2+3+5
	u64 constexpr limit = 100000;
	while(true){
	    u32 prime = *++pi;
	    if(prime > limit)
		return sum;
	    u32 a_n = construct_lowrepunit(prime);
	    while(a_n % 5 == 0)
		a_n /= 5;
	    if((a_n & (a_n - 1)) != 0)
		sum += prime;
	}
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set13
    {P(130),P(131),P(132),P(133)};
}
