// Copyright 2014 David Adler

#include <array>
#include <vector>
#include <algorithm>
#include <cassert>

#include <set>

#include "util.hh"
#include "atkin.hh"

namespace {
    using namespace euler;
    u64 problem20(){
	bigint fac;
	mpz_fac_ui(fac.get_mpz_t(), 100);
	return digit_sum(fac);
    }
    u64 problem21(){
	u64 constexpr limit = 10000;
	std::vector<u64> vec(1);
	for(u64 i = 1; i < limit; ++i)
	    vec.push_back(divisor_sum(i) - i);
	u64 sum = 0;
	for(u64 i = 1; i < limit; ++i){
	    u64 const ds = vec[i];
	    if(ds != i && (ds < limit ? vec[ds] == i : divisor_sum(ds) == i))
		sum += i;
	}
	return sum;
    }
    std::array<char const *const, 5163> input22 {{
#include "names.include"
    }};
    u64 problem22(){
	u64 sum = 0;
	u64 pos = 0;
	std::vector<std::string> names(input22.cbegin(), input22.cend());
	std::sort(names.begin(), names.end());
	for(auto str : names){
	    u64 psum = 0;
	    for(char ch : str)
		psum += ch + 1 - 'A';
	    sum += psum * ++pos;
	}
	return sum;
    }
    u64 problem23(){
	static u64 constexpr limit = 28124;
	std::vector<u64> abundant;
	for(u64 i = 1; i < limit; ++i)
	    if(divisor_sum(i) - i > i)
		abundant.push_back(i);
	std::vector<bool> isSum(limit, false);
	auto outer = abundant.cbegin();
	while(outer != abundant.cend()){
	    auto inner = outer;
	    do {
		u64 const v = *inner + *outer;
		if(v < limit)
		    isSum[v] = true;
	    } while(++inner != abundant.cend());
	    ++outer;
	}
	u64 sum = 0;
	for(u64 i = 0; i < limit; ++i)
	    if(!isSum[i])
		sum += i;
	return sum;
    }
    std::string problem24(){
	std::array<char, 10> arr {{'0','1','2','3','4','5','6','7','8','9'}};
	for(u32 i = 1; i < 1000000; ++i){
	    if(!std::next_permutation(arr.begin(), arr.end()))
		assert(false);
	}
	return { arr.begin(), arr.end() };
    }
    u64 problem25(){
	bigint a = 0;
	bigint b = 1;
	bigint target;
	mpz_ui_pow_ui(target.get_mpz_t(), 10, 999);
	u64 i;
	for(i = 0; a < target; ++i){
	    mpz_swap(a.get_mpz_t(), b.get_mpz_t());
	    b += a;
	}
	return i;
    }
    u64 rec_rep(u64 x){
	//remove 2s and 5s
	while(x % 2 == 0)
	    x /= 2;
	while(x % 5 == 0)
	    x /= 5;
	if(x == 1)
	    return 0;
	u64 start = 1;
	u64 const n = x;
	while(start < x)
	    start *= 10;
	x = start;
	u64 i = 0;
	do {
	    ++i;
	    x = (x % n) * 10;
	} while(x != start);
	return i;
    }
    u64 problem26(){
	u64 max = 0;
	u64 v = 0;
	for(u64 i = 2; i < 1000; ++i){
	    u64 const rr = rec_rep(i);
	    if(rr > max){
		max = rr;
		v = i;
	    }
	}
	return v;
    }
    long problem27(){
	long max = 0;
	long val = 0;
	prime_iterator b_iter;
	for(long b = *b_iter++; b < 1000; b = *b_iter++){
	    for(long a = -999; a < 1000; ++a){
		long n = 1; // case n=0 is implied by b being prime
		long quad = 1 + a + b;
		while(quad > 0 && factors((u64)quad).size() == 1)
		    quad += a + 2*n++ + 1;
		if(n > max){
		    max = n; //actually one more than satisfying conditions
		    val = a*b;
		}
	    }
	}
	return val;
    }
    u64 problem28(){
	u64 sum = 0;
	for(u32 n = 1; n <= 500; ++n)
	    sum += 4*n*n+n+1;
	sum *= 4;
	sum += 1;
	return sum;
    }
    u64 problem29(){
	u64 ct = 0;
	std::set<bigint> set;
	bigint p;
	for(u64 a = 2; a <= 100; ++a)
	    for(u64 b = 2; b <= 100; ++b){
		mpz_ui_pow_ui(p.get_mpz_t(), a, b);
		auto iter = set.find(p);
		if(iter == set.cend()){
		    ++ct;
		    set.insert(p);
		}
	    }
	return ct;
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set2
    {P(20), P(21), P(22), P(23), P(24), P(25), P(26), P(27), P(28), P(29)};
}
