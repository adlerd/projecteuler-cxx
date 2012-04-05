#include <algorithm>

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
#define P(x) {x, &problem ## x}
    std::list<problem> set5
    {{P(50),P(51)}};
}
