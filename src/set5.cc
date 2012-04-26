#include <algorithm>
#include <array>

#include "set5.hh"
#include "util.hh"
#include "atkin.hh"

namespace euler {
    std::string problem50(){
	ulong max_ct = 0;
	ulong max_prime = 0;
	for(prime_iterator start; *start < 1000; ++start){
	    ulong sum = 0;
	    ulong ct = 0;
	    for(prime_iterator end(start); sum < 1000000; ++end){
		if(ct > max_ct && is_prime(sum)){
		    max_ct = ct;
		    max_prime = sum;
		}
		++ct;
		sum += *end;
	    }
	}
	return std::to_string(max_prime);
    }
    template <class Iterator, class Object>
    std::list<ulong> find_all(Iterator begin, Iterator end, Object what){
	std::list<ulong> indices;
	for(auto iter = std::find(begin, end, what);
		iter != end;
		iter = std::find(iter + 1, end, what))
	    indices.push_back(std::distance(begin, iter));
	return indices;
    }
    std::string problem51(){
	for(prime_iterator pi(10); true; ++pi){
	    std::vector<uchar> digits(digit_iterator(*pi), digit_iterator(0));
	    std::reverse(digits.begin(), digits.end());
	    for(uchar base_digit : {0, 1, 2}){
		std::list<ulong> indices =
		    find_all(digits.begin(), digits.end(), base_digit);
		ulong const limit = 1 << indices.size();
		for(uint i = 1; i < limit; ++i){
		    uchar count = 1;
		    std::list<ulong> effective_indices;
		    {
			uint j = i;
			for(ulong x : indices){
			    if(j & 1)
				effective_indices.push_back(x);
			    j >>= 1;
			}
		    }
		    std::vector<uchar> new_digits(digits);
		    for(uchar new_digit = base_digit + 1;
			    new_digit < 10; ++new_digit){
			for(ulong x : effective_indices)
			    new_digits[x] = new_digit;
			if(is_prime(from_digits(new_digits.begin(), new_digits.end()))){
			    ++count;
			    if(count >= 8)
				return std::to_string(*pi);
			} else if(count + 9 - i < 8){
			    break;
			}
		    }
		}
	    }
	}
    }
    std::string problem52(){
	for(ulong i = 1; true; ++i){
	    std::vector<uchar> one(digit_iterator(i), digit_iterator(0));
	    std::vector<uchar> other(digit_iterator(6*i), digit_iterator(0));
	    if(one.size() == other.size()){
		std::sort(one.begin(), one.end());
		std::sort(other.begin(), other.end());
		ulong n = 6;
		while(std::equal(one.begin(), one.end(), other.begin())){
		    if(--n == 1)
			return std::to_string(i);
		    other.assign(digit_iterator(n*i), digit_iterator(0));
		    std::sort(other.begin(), other.end());
		}
	    }
	}
    }
    std::string problem53(){
	ulong ct = 0;
	for(ulong n = 23; n <= 100; ++n){
	    ulong first_r = 0;
	    ulong ncr = 1;
	    for(ulong r = 1; first_r == 0; ++r){
		ncr *= (n - r + 1);
		ncr /= r;
		if(ncr > 1000000)
		    first_r = r;
	    }
	    ct += n + 1 - 2 * first_r;
	}
	return std::to_string(ct);
    }
    uchar constexpr bad_card_lookup = 255;
#define B bad_card_lookup
    std::array<uchar, 85> const char_lookup = {{
	B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
	B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
	B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
	B, B, 0, 1, 2, 3, 4, 5, 6, 7, B, B, B, B, B, B,
	B,12, B, 0, 1, B, B, B, 2, B, 9,11, B, B, B, B,
	B,10, B, 3, 8 }};
#undef B
    ulong next_hand_value(char const *input){
	uchar value_counts[13] {0,0,0,0,0,0,0,0,0,0,0,0,0};
	uchar suit_counts[4] {0,0,0,0};
	uchar four = 13;
	uchar trip = 13;
	uchar doub = 13;
	uchar second_doub = 13;
	bool straight = true;
	bool flush = false;
	for(uint c = 0; c < 5; ++c){
	    char c = *input++;
	    assert(c >= 0 && c <= 'T');
	    uchar v = char_lookup[c];
	    assert(v < 13);
	    ++value_counts[v];
	    c = *input++;
	    assert(c >= 0 && c <= 'T');
	    v = char_lookup[c];
	    assert(v < 4);
	    ++suit_counts[v];
	    c = *input++;
	    assert(c == ' ');
	}
	uint more = 5;
	for(uint v = 0; more; ++v){
	    assert(v < 13);
	    uint const ct = value_counts[v];
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
	for(uint s = 0; s < 4; ++s){
	    uint const ct = suit_counts[s];
	    if(ct == 5)
		flush = true;
	    if(ct != 0)
		break;
	}
	// value is a 8-digit base 13 number, with these digits:
	ulong value;
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
	uint extras = 0;
	uint seen = 0;
	for(int v = 12; seen < 5; --v){
	    uint ct = value_counts[v];
	    if(ct > 0){
		value *= 13;
		value += v;
		extras += ct - 1;
		seen += ct;
	    }
	}
	for(uint i = 0; i < extras; ++i)
	    value *= 13;
	return value;
    }
    char const *const input54 =
#include "poker.include"
	;
    std::string problem54(){
	char const *input = input54;
	uint one_count = 0;
	for(uint i = 0; i < 1000; ++i){
	    ulong const one = next_hand_value(input);
	    input += 15;
	    ulong const two = next_hand_value(input);
	    input += 15;
	    if(one > two)
		++one_count;
	}
	return std::to_string(one_count);
    }
    std::string problem55(){
	ulong lychrel_count = 0;
	for(ulong i = 1; i < 10000; ++i){
	    ulong n = i;
	    for(uint c = 0; true; ++c){
		auto nstr = std::to_string(n);
		if(c > 0 && is_palindrome(nstr)){
		    break;
		} else if(c == 50){
		    ++lychrel_count;
		    break;
		}
		n += from_digits(digit_iterator(n), digit_iterator(0));
	    }
	}
	return std::to_string(lychrel_count);
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set5
    {{P(50),P(51),P(52),P(53),P(54),P(55)}};
}
