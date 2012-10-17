#include "set7.hh"
#include "util.hh"
#include <list>
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
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set7
    {{P(70)}};
}
