#include "set7.hh"
#include "util.hh"
#include <list>
#include <forward_list>
#include <array>
#include <algorithm>

namespace euler {
    bool is_digit_permutation(ulong a, ulong b){
	std::vector<uchar> digs_a(digit_iterator(a), digit_iterator(0));
	std::vector<uchar> digs_b(digit_iterator(b), digit_iterator(0));
	return digs_a.size() == digs_b.size() &&
	    std::is_permutation(digs_a.begin(), digs_a.end(), digs_b.begin());
    }
    ulong problem70(){
	double min_ratio = 87109.0 / 79180;
	ulong min_n = 87109;
	for(ulong n = 3; n < 10000000; ++n){
	    ulong tot = totient(n);
	    double ratio = ((double) n) / tot;
	    if(ratio < min_ratio && is_digit_permutation(n,tot)){
		min_ratio = ratio;
		min_n = n;
	    }
	}
	return min_n;
    }
    uint problem71(){
	uint target_n = 3;
	uint target_d = 7;
	uint max_d = 1000000;
	uint best_n = target_n - 1;
	uint best_d = target_d;
	{
	    uint g = gcd(best_n, best_d);
	    best_n /= g;
	    best_d /= g;
	}
	for(uint d = 2; d <= max_d; ++d){
	    uint n = d * target_n / target_d;
	    if(gcd(n, d) == 1 && n*best_d > best_n * d){
		if(d == target_d)
		    continue;
		best_d = d;
		best_n = n;
	    }
	}
	return best_n;
    }
    ulong problem72(){
	ulong sum = 0;
	for(uint i = 2; i <= 1000000; ++i)
	    sum += totient(i);
	return sum;
    }
    uint problem73(){
	uint ct = 0;
	for(uint d = 1; d <= 12000; ++d){
	    uint n = d/3;
	    while(2*++n < d)
		if(gcd(n,d) == 1)
		    ++ct;
	}
	return ct;
    }
    uint problem74(){
	std::array<uchar, 2178000> lens;
	std::array<uint, 10> facts;
	facts[0] = 1;
	for(uint i = 1, n = 1; i < 10; ++i)
	    facts[i] = (n *= i);
	lens[169] = lens[363601] = lens[1545] = 3;
	lens[871] = lens[45361] = 2;
	lens[872] = lens[45362] = 2;
	uint ct = 0;
	for(uint i = 1; i < 1000000; ++i){
	    uint n = i;
	    uint l = lens[n];
	    std::forward_list<uint> stack;
	    while(l == 0){
		stack.push_front(n);
		digit_iterator di(n);
		n = 0;
		while(di != digit_iterator(0))
		    n += facts[*di++];
		if(n == stack.front()){
		    l = 1;
		} else if(lens[n]){
		    if(lens[n] >= 60)
			l = 61;
		    else
			l = lens[n] + 1;
		}
	    }
	    while(!stack.empty()){
		lens[stack.front()] = l;
		stack.pop_front();
		if(l == 60)
		    ++ct;
		if(l < 61)
		    ++l;
	    }
	}
	return ct;
    }
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set7
    {{P(70),P(71),P(72),P(73),P(74)}};
}
