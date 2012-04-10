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
#define P(x) {x, &problem ## x}
    std::list<problem> set5
    {{P(50),P(51),P(52),P(53)}};
}
