// Copyright 2014 David Adler

#include <algorithm>
#include <array>

#include "util.hh"
#include "atkin.hh"

namespace {
    using namespace euler;
    u64 problem50(){
	u64 max_ct = 0;
	u64 max_prime = 0;
	for(prime_iterator start; *start < 1000; ++start){
	    u64 sum = 0;
	    u64 ct = 0;
	    for(prime_iterator end(start); sum < 1000000; ++end){
		if(ct > max_ct && is_prime(sum)){
		    max_ct = ct;
		    max_prime = sum;
		}
		++ct;
		sum += *end;
	    }
	}
	return max_prime;
    }
    template <class Iterator, class Object>
    std::list<u64> find_all(Iterator begin, Iterator end, Object what){
	std::list<u64> indices;
	for(auto iter = std::find(begin, end, what);
		iter != end;
		iter = std::find(iter + 1, end, what))
	    indices.push_back(std::distance(begin, iter));
	return indices;
    }
    u64 problem51(){
	for(prime_iterator pi(10); true; ++pi){
	    std::vector<u8> digits(digit_iterator(*pi), digit_iterator(0));
	    std::reverse(digits.begin(), digits.end());
	    for(u8 base_digit : {0, 1, 2}){
		std::list<u64> indices =
		    find_all(digits.begin(), digits.end(), base_digit);
		u64 const limit = 1 << indices.size();
		for(u32 i = 1; i < limit; ++i){
		    u8 count = 1;
		    std::list<u64> effective_indices;
		    {
			u32 j = i;
			for(u64 x : indices){
			    if(j & 1)
				effective_indices.push_back(x);
			    j >>= 1;
			}
		    }
		    std::vector<u8> new_digits(digits);
		    for(u8 new_digit = base_digit + 1;
			    new_digit < 10; ++new_digit){
			for(u64 x : effective_indices)
			    new_digits[x] = new_digit;
			if(is_prime(from_digits(new_digits.begin(), new_digits.end()))){
			    ++count;
			    if(count >= 8)
				return *pi;
			} else if(count + 9 - i < 8){
			    break;
			}
		    }
		}
	    }
	}
    }
    u64 problem52(){
	for(u64 i = 1; true; ++i){
	    std::vector<u8> one(digit_iterator(i), digit_iterator(0));
	    std::vector<u8> other(digit_iterator(6*i), digit_iterator(0));
	    if(one.size() == other.size()){
		std::sort(one.begin(), one.end());
		std::sort(other.begin(), other.end());
		u64 n = 6;
		while(std::equal(one.begin(), one.end(), other.begin())){
		    if(--n == 1)
			return i;
		    other.assign(digit_iterator(n*i), digit_iterator(0));
		    std::sort(other.begin(), other.end());
		}
	    }
	}
    }
    u64 problem53(){
	u64 ct = 0;
	for(u64 n = 23; n <= 100; ++n){
	    u64 first_r = 0;
	    u64 ncr = 1;
	    for(u64 r = 1; first_r == 0; ++r){
		ncr *= (n - r + 1);
		ncr /= r;
		if(ncr > 1000000)
		    first_r = r;
	    }
	    ct += n + 1 - 2 * first_r;
	}
	return ct;
    }
    u8 constexpr bad_card_lookup = 255;
#define B bad_card_lookup
    std::array<u8, 85> const char_lookup = {{
	B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
	B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
	B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
	B, B, 0, 1, 2, 3, 4, 5, 6, 7, B, B, B, B, B, B,
	B,12, B, 0, 1, B, B, B, 2, B, 9,11, B, B, B, B,
	B,10, B, 3, 8 }};
#undef B
    u64 next_hand_value(char const *input){
	u8 value_counts[13] {0,0,0,0,0,0,0,0,0,0,0,0,0};
	u8 suit_counts[4] {0,0,0,0};
	u8 four = 13;
	u8 trip = 13;
	u8 doub = 13;
	u8 second_doub = 13;
	bool straight = true;
	bool flush = false;
	for(u32 c = 0; c < 5; ++c){
	    char ch = *input++;
	    assert(ch >= 0 && ch <= 'T');
	    u8 v = char_lookup[ch];
	    assert(v < 13);
	    ++value_counts[v];
	    ch = *input++;
	    assert(ch >= 0 && ch <= 'T');
	    v = char_lookup[ch];
	    assert(v < 4);
	    ++suit_counts[v];
	    ch = *input++;
	    assert(ch == ' ');
	}
	u32 more = 5;
	for(u32 v = 0; more; ++v){
	    assert(v < 13);
	    u32 const ct = value_counts[v];
	    switch(ct){
	    case 0:
		if(more != 5)
		    straight = false;
		break;
	    case 1:
		break;
	    case 2:
		straight = false;
		if(doub == 13)
		    doub = v;
		else {
		    if(doub < v){
			second_doub = doub;
			doub = v;
		    } else {
			second_doub = v;
		    }
		}
		break;
	    case 3:
		straight = false;
		trip = v;
		break;
	    case 4:
		straight = false;
		four = v;
		break;
	    default:
		assert(false);
	    }
	    more -= ct;
	}
	for(u32 s = 0; s < 4; ++s){
	    u32 const ct = suit_counts[s];
	    if(ct == 5)
		flush = true;
	    if(ct != 0)
		break;
	}
	// value is a 8-digit base 13 number, with these digits:
	u64 value;
	if(flush && straight)
	    value = 12 * 13 * 13;
	else if(four != 13)
	    value = (11 * 13 + four) * 13;
	else if(trip != 13 && doub != 13)
	    value = (10 * 13 + trip) * 13;
	else if(flush)
	    value = (9 * 13) * 13;
	else if(straight)
	    value = (8 * 13) * 13;
	else if(trip != 13)
	    value = (7 * 13 + trip) * 13;
	else if(second_doub != 13)
	    value = (6 * 13 + doub) * 13 + second_doub;
	else if(doub != 13)
	    value = (5 * 13 + doub) * 13;
	else
	    value = 0;
	u32 extras = 0;
	u32 seen = 0;
	for(u32 v = 12; seen < 5; --v){
	    u32 ct = value_counts[v];
	    if(ct > 0){
		value *= 13;
		value += v;
		extras += ct - 1;
		seen += ct;
	    }
	}
	for(u32 i = 0; i < extras; ++i)
	    value *= 13;
	return value;
    }
    char const *const input54 =
#include "poker.include"
	;
    u64 problem54(){
	char const *input = input54;
	u32 one_count = 0;
	for(u32 i = 0; i < 1000; ++i){
	    u64 const one = next_hand_value(input);
	    input += 15;
	    u64 const two = next_hand_value(input);
	    input += 15;
	    if(one > two)
		++one_count;
	}
	return one_count;
    }
    u64 problem55(){
	u64 lychrel_count = 0;
	for(u64 i = 1; i < 10000; ++i){
	    u64 n = i;
	    for(u32 c = 0; true; ++c){
		auto nstr = n;
		if(c > 0 && is_palindrome(std::to_string(nstr))){
		    break;
		} else if(c == 50){
		    ++lychrel_count;
		    break;
		}
		n += from_digits(digit_iterator(n), digit_iterator(0));
	    }
	}
	return lychrel_count;
    }
    u64 problem56(){
	bigint big;
	u64 max_sum = 0;
	for(u32 a = 1; a < 100; ++a)
	    for(u32 b = 1; b < 100; ++b){
		mpz_ui_pow_ui(big.get_mpz_t(), a, b);
		max_sum = std::max(max_sum, digit_sum(big));
	    }
	return max_sum;
    }
    u64 problem57(){
	typedef mpq_class bigq;
	bigq q(1);
	u64 ct = 0;
	for(u32 i = 0; i < 1000; ++i){
	    q = 1 + 1 / (1 + q);
	    if(q > 1 && q.get_num().get_str().size() > q.get_den().get_str().size())
		++ct;
	}
	return ct;
    }
    u64 problem58(){
	u64 prime_ct = 0;
	for(u32 n = 1; true; ++n){
	    u64 base = 1+2*n*(2*n-1);
	    for(u32 rep = 0; rep < 3; ++rep){
		if(is_prime(base))
		    ++prime_ct;
		base += 2*n;
	    }
	    if(10*prime_ct < (4*n + 1))
		return 2*n+1;
	}
    }
    std::array<std::string, 5> const common_words = {{"the", "and", "of", "to", "in"}};
    std::array<u8, 1201> const input59 = {{
#include "cipher1.include"
    }};
    bool input59_check(u32 const offset, u8 const xchar){
	for(u32 i = offset; i < input59.size(); i += 3){
	    u8 const c = input59[i] xor xchar;
	    if(c > 'z' || c < ' ')
		return false;
	}
	return true;
    }
    void input59_transform(u32 const offset, u8 const xchar, std::array<u8, 1201>& out){
	for(u32 i = offset; i < input59.size(); i += 3)
	    out[i] = input59[i] xor xchar;
    }
    u64 problem59(){
	std::vector<u8> c1s;
	for(u8 c1 = 'a'; c1 <= 'z'; ++c1)
	    if(input59_check(0, c1))
		c1s.push_back(c1);
	std::vector<u8> c2s;
	for(u8 c2 = 'a'; c2 <= 'z'; ++c2)
	    if(input59_check(1, c2))
		c2s.push_back(c2);
	std::vector<u8> c3s;
	for(u8 c3 = 'a'; c3 <= 'z'; ++c3)
	    if(input59_check(2, c3))
		c3s.push_back(c3);
	std::array<u8, 1201> check;
	for(u8 c1 : c1s){
	    input59_transform(0, c1, check);
	    for(u8 c2 : c2s){
		input59_transform(1, c2, check);
		for(u8 c3 : c3s){
		    input59_transform(2, c3, check);
		    bool good = true;
		    for(auto word : common_words){
			if(std::search(check.cbegin(), check.cend(), word.cbegin(), word.cend()) == check.cend()){
			    good = false;
			    break;
			}
		    }
		    if(good){
			u64 ret = 0;
			for(u8 ch : check)
			    ret += ch;
			return ret;
		    }
		}
	    }
	}
	throw std::logic_error("Solution not found for problem 59!");
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set5
    {{P(50),P(51),P(52),P(53),P(54),P(55),P(56),P(57),P(58),P(59)}};
}
