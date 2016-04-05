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
    u64 problem134(){
	u32 constexpr target = 1000000;
	prime_iterator pi(6);
	u32 prev_prime;
	u32 prime = 5;
	u64 sum = 0;
	while(prime < target){
	    prev_prime = prime;
	    prime = *pi++;
	    u64 digits = prev_prime;
	    u64 working = prime;
	    u64 ten = 1;
	    while(digits > 0){
		if(working % 2 != digits % 2)
		    working += prime;
		while(working % 10 != digits % 10)
		    working += 2*prime;
		digits /= 10;
		working /= 10;
		ten *= 10;
	    }
	    working *= ten;
	    working += prev_prime;
	    sum += working;
	}
	return sum;
    }
    u32 problem135(){
	u64 constexpr limit = 1000000;
	std::vector<u8> ct(limit);
	for(u64 a = 1; a * 4 < limit * 3; ++a){
	    for(u64 k = (a-1)/3+1; ; ++k){
		u64 n = (3*k-a)*(a+k);
		if(n >= limit)
		    break;
		++ct[n];
	    }
	}
	u32 ret = 0;
	for(u8 c : ct){
	    if(c == 10)
		++ret;
	}
	return ret;
    }
    u32 problem136(){
	u64 constexpr limit = 50000000;
	std::vector<u8> ct(limit);
	for(u64 a = 1; a * 4 < limit * 3; ++a){
	    for(u64 k = (a-1)/3+1; ; ++k){
		u64 n = (3*k-a)*(a+k);
		if(n >= limit)
		    break;
		if(ct[n] > 1)
		    continue;
		++ct[n];
	    }
	}
	u32 ret = 0;
	for(u8 c : ct){
	    if(c == 1)
		++ret;
	}
	return ret;
    }
    bigint problem137(){
	// the limit of the generating series for Fibonacci numbers
	// is x/(1-x-x^2); if the value of this is an integer n then
	// x is rational iff 1+2n+5n^2 is a square
	/*
	for(u64 a = 1; a < (1<<30) ; ++a){
	    if(isqrt(1+2*a+5*a*a) != 0)
		std::cerr << a << std::endl;
	}*/
	// oeis gives F(2n)F(2n+1) for the above sequence, so...
	bigint a = 0;
	bigint b = 1;
	u8 i = 1;
	while(i < 31){
	    a += b;
	    std::swap(a,b);
	    ++i;
	}
	return a*b;
    }
    u64 problem138(){
	// According to P. Shiu's The Shapes and Sizes of Pythagorean
	// Triangles, given a right triangle with smallest angle theta,
	// convergents m/n of tan(theta)+sec(theta) give parameters m and n.
	// We would like to find Pythagorean approximations to the right
	// triangle with legs 1 and 2 units. The smallest angle is opposite the
	// shorter leg, so tan(theta)=1/2 while sec(theta)=sqrt(5)/2. The sum
	// of these is the golden ratio, so convergents are ratios of
	// successive fibonacci terms, giving m and n directly.
	// We have to reduce to primitive terms, and, per Shiu, the result is
	// 'in order'.
	// Inspection shows that not all results are valid, so we filter.
	u64 m = 1;
	u64 n = 1;
	u8 ct = 0;
	u64 sum = 0;
	while(ct < 12){
	    n += m; std::swap(n,m);
	    u64 b = m*m-n*n; // actually half the base
	    u64 h = 2*m*n;
	    u64 l = m*m+n*n;
	    u64 g = gcd(gcd(b,h),l);
	    b = 2*(b/g);
	    h = h/g;
	    l = l/g;
	    if(b == h+1 || b+1 == h){
		sum += l;
		++ct;
	    }
	}
	return sum;
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set13
    {P(130),P(131),P(132),P(133),P(134),P(135),P(136),P(137),P(138)};
}
