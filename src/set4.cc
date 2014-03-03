#include <array>
#include <algorithm>
#include <bitset>

#include "util.hh"

#include "atkin.hh"

namespace {
    using namespace euler;
    std::string problem40(){
	u64 p = 1;
	std::vector<u8> rdigs = {0};
	auto iter = rdigs.rbegin();
	u64 n = 0;
	u64 i = 0;
	u64 target = 1;
	while(target <= 1000000){
	    while(i < target){
		if(++iter == rdigs.rend()){
		    rdigs.assign(digit_iterator(++n), digit_iterator());
		    iter = rdigs.rbegin();
		}
		++i;
	    }
	    p *= *iter;
	    target *= 10;
	}
	return std::to_string(p);
    }
    std::string problem41(){
	std::array<u8, 9> digs = {{ 9, 8, 7, 6, 5, 4, 3, 2, 1 }};
	u64 r_ct = 0;
	while(true){
	    if(!std::prev_permutation(digs.begin() + r_ct, digs.end()))
		++r_ct;
	    u64 n = from_digits(digs.begin() + r_ct, digs.end());
	    if(is_prime(n))
		return std::to_string(n);
	}
    }
    std::string problem42(){
	u64 ct = 0;
	for(std::string str : input_words){
	    u64 csum = 0;
	    for(char c : str)
		csum += c + 1 - 'A';
	    if(is_figurate<3>(csum))
		++ct;
	}
	return std::to_string(ct);
    }
    std::string problem43(){
	// use 1, 0, 2... to skip 0, 1, 2 through 0, 9, 8
	std::array<u8, 10> digs = {{1, 0, 2, 3, 4, 5, 6, 7, 8, 9}};
	u64 sum = 0;
	do {
	    u64 rsum;
	    if(digs[3] % 2 != 0)
		goto next;
	    if((digs[2] + digs[3] + digs[4]) % 3 != 0)
		goto next;
	    if(digs[5] % 5 != 0)
		goto next;
	    rsum = digs[4] * 100 + digs[5] * 10 + digs[6];
	    if(rsum % 7 != 0)
		goto next;
	    rsum = (rsum % 100) * 10 + digs[7];
	    if(rsum % 11 != 0)
		goto next;
	    rsum = (rsum % 100) * 10 + digs[8];
	    if(rsum % 13 != 0)
		goto next;
	    rsum = (rsum % 100) * 10 + digs[9];
	    if(rsum % 17 != 0)
		goto next;
	    sum += from_digits(digs.cbegin(), digs.cend());
next:;
	} while(std::next_permutation(digs.begin(), digs.end()));
	return std::to_string(sum);
    }
    std::string problem44(){
	std::vector<u64> lowers;
	for(figurate::pent_iter upper; true; ++upper){
	    for(u64 lower : lowers){
		u64 const diff = *upper - lower;
		u64 const sum = *upper + lower;
		if(is_figurate<5>(diff) && is_figurate<5>(sum))
		    return std::to_string(diff);
	    }
	    lowers.push_back(*upper);
	}
    }
    std::string problem45(){
	figurate::tri_iter ti(286);
	figurate::pent_iter pi(166);
	figurate::hex_iter hi(144);
	while(true){
	    if(*ti < *pi)
		++ti;
	    else if(*ti > *pi)
		++pi;
	    else if(*hi < *pi)
		++hi;
	    else if(*hi > *pi)
		{++pi; ++ti;}
	    else
		return std::to_string(*hi);
	}
    }
    std::string problem46(){
	for(u64 i = 35; true; i += 2){
	    bool good = false;
	    for(figurate::square_iter si(0); !good && (*si)*2 < i; ++si){
		u64 diff = i - (*si)*2;
		if(is_prime(diff))
		    good = true;
	    }
	    if(!good)
		return std::to_string(i);
	}
    }
    std::string problem47(){
	std::bitset<4> bits(0);
	for(u64 i = 1; true; ++i){
	    bits <<= 1;
	    bits[0] = (ct_factors(i).size() == 4);
	    if(bits.all())
		return std::to_string(i - 3);
	}
    }
    std::string problem48(){
	u64 constexpr mod = 10000000000;
	bigint mod_bi(mod);
	bigint base;
	u64 sum = 0;
	for(u64 i = 1; i < 1001; ++i){
	    base = i;
	    mpz_powm_ui(base.get_mpz_t(), base.get_mpz_t(), i,
		    mod_bi.get_mpz_t());
	    sum = (sum + base.get_ui()) % mod;
	}
	return std::to_string(sum);
    }
    std::string problem49(){
	for(prime_iterator pi(1000); *pi < 3340; ++pi){
	    u64 const p1 = *pi;
	    u64 const p2 = p1 + 3330;
	    u64 const p3 = p2 + 3330;
	    if(p1 != 1487 && is_prime(p2) && is_prime(p3)
		    && is_permutation(digit_iterator(p1), digit_iterator(0),
			digit_iterator(p2))
		    && is_permutation(digit_iterator(p1), digit_iterator(0),
			digit_iterator(p3))){
		return std::to_string(p1) + std::to_string(p2)
		    + std::to_string(p3);
	    }
	}
	return "No solution found!";
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set4
    {{P(40),P(41),P(42),P(43),P(44),P(45),P(46),P(47),P(48),P(49)}};
}
