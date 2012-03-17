#include <array>
#include <vector>
#include <algorithm>
#include <cassert>

#include <set>

#include "set2.hh"
#include "util.hh"
#include "atkin.hh"

namespace euler {
    std::string problem20(){
	bigint fac;
	mpz_fac_ui(fac.get_mpz_t(), 100);
	return std::to_string(digit_sum(fac));
    }
    std::string problem21(){
	ulong constexpr limit = 10000;
	std::vector<ulong> vec(1);
	for(ulong i = 1; i < limit; ++i)
	    vec.push_back(divisor_sum(i) - i);
	ulong sum = 0;
	for(ulong i = 1; i < limit; ++i){
	    ulong const ds = vec[i];
	    if(ds != i && (ds < limit ? vec[ds] == i : divisor_sum(ds) == i))
		sum += i;
	}
	return std::to_string(sum);
    }
    std::array<char const *const, 5163> input22 {{
#include "names.include"
    }};
    std::string problem22(){
	ulong sum = 0;
	ulong pos = 0;
	std::vector<std::string> names(input22.cbegin(), input22.cend());
	std::sort(names.begin(), names.end());
	for(auto str : names){
	    ulong psum = 0;
	    for(char ch : str)
		psum += ch + 1 - 'A';
	    sum += psum * ++pos;
	}
	return std::to_string(sum);
    }
    std::string problem23(){
	static ulong constexpr limit = 28124;
	std::vector<ulong> abundant;
	for(ulong i = 1; i < limit; ++i)
	    if(divisor_sum(i) - i > i)
		abundant.push_back(i);
	std::vector<bool> isSum(limit, false);
	auto outer = abundant.cbegin();
	while(outer != abundant.cend()){
	    auto inner = outer;
	    do {
		ulong const v = *inner + *outer;
		if(v < limit)
		    isSum[v] = true;
	    } while(++inner != abundant.cend());
	    ++outer;
	}
	ulong sum = 0;
	for(ulong i = 0; i < limit; ++i)
	    if(!isSum[i])
		sum += i;
	return std::to_string(sum);
    }
    std::string problem24(){
	std::array<char, 10> arr {{'0','1','2','3','4','5','6','7','8','9'}};
	for(int i = 1; i < 1000000; ++i){
	    bool const ret = std::next_permutation(arr.begin(), arr.end());
	    assert(ret);
	}
	return { arr.begin(), arr.end() };
    }
    std::string problem25(){
	bigint a = 0;
	bigint b = 1;
	bigint target;
	mpz_ui_pow_ui(target.get_mpz_t(), 10, 999);
	ulong i;
	for(i = 0; a < target; ++i){
	    mpz_swap(a.get_mpz_t(), b.get_mpz_t());
	    b += a;
	}
	return std::to_string(i);
    }
    ulong rec_rep(ulong x){
	//remove 2s and 5s
	while(x % 2 == 0)
	    x /= 2;
	while(x % 5 == 0)
	    x /= 5;
	if(x == 1)
	    return 0;
	ulong start = 1;
	ulong const n = x;
	while(start < x)
	    start *= 10;
	x = start;
	ulong i = 0;
	do {
	    ++i;
	    x = (x % n) * 10;
	} while(x != start);
	return i;
    }
    std::string problem26(){
	ulong max = 0;
	ulong v = 0;
	for(ulong i = 2; i < 1000; ++i){
	    ulong const rr = rec_rep(i);
	    if(rr > max){
		max = rr;
		v = i;
	    }
	}
	return std::to_string(v);
    }
    std::string problem27(){
	long max = 0;
	long val = 0;
	prime_iterator b_iter;
	for(long b = *b_iter++; b < 1000; b = *b_iter++){
	    for(long a = -999; a < 1000; ++a){
		long n = 1; // case n=0 is implied by b being prime
		long quad = 1 + a + b;
		while(quad > 0 && factors((ulong)quad).size() == 1)
		    quad += a + 2*n++ + 1;
		if(n > max){
		    max = n; //actually one more than satisfying conditions
		    val = a*b;
		}
	    }
	}
	return std::to_string(val);
    }
    std::string problem28(){
	ulong sum = 0;
	for(uint n = 1; n <= 500; ++n)
	    sum += 4*n*n+n+1;
	sum *= 4;
	sum += 1;
	return std::to_string(sum);
    }
    std::string problem29(){
	ulong ct = 0;
	std::set<bigint> set;
	bigint p;
	for(ulong a = 2; a <= 100; ++a)
	    for(ulong b = 2; b <= 100; ++b){
		mpz_ui_pow_ui(p.get_mpz_t(), a, b);
		auto iter = set.find(p);
		if(iter == set.cend()){
		    ++ct;
		    set.insert(p);
		}
	    }
	return std::to_string(ct);
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set2
    {{P(20), P(21), P(22), P(23), P(24), P(25), P(26), P(27), P(28), P(29)}};
}
