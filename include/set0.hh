#ifndef _SET0_HH_
#define _SET0_HH_

#include "util.hh"
#include <list>
#include <cassert>
#include <type_traits>

namespace euler {
    extern std::list<problem> set0;
    template <class Iter>
	ulong high_subseq_prod(Iter first, Iter const last, ulong len){
	    ulong zct = 0;
	    ulong prod = 1;
	    typename std::remove_reference<Iter>::type middle(first);
	    for(ulong i = 0; i < len; ++i){
		if(middle == last)
		    return 0;
		ulong const p = *middle++;
		if(p == 0)
		    ++zct;
		else
		    prod *= p;
	    }
	    assert(middle != first);
	    ulong high = zct == 0 ? prod : 0;
	    while(middle != last){
		ulong const p = *middle++;
		ulong const d = *first++;
		if(d == 0)
		    --zct;
		else
		    prod /= d;
		if(p == 0)
		    ++zct;
		else
		    prod *= p;
		if(zct == 0 && prod > high)
		    high = prod;
	    }
	    return high;
	}
}

#endif
