// Copyright 2014 David Adler

#include "util.hh"

namespace {
    using namespace euler;
    bigint problem110(){
	using namespace euler108;
	u64 constexpr minlim = 4000000;
	u64 constexpr search = 20000;
	bigint min = smallestN(minlim);
	bigint tmp;
	for(u64 t = minlim + 1; t < minlim + search; ++t){
	    tmp = smallestN(t);
	    if(tmp < min)
		std::swap(tmp, min);
	}
	return min;
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set11
    {P(110)};
}
