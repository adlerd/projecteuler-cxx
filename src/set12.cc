// Copyright 2014 David Adler

#include "util.hh"

namespace {
    using namespace euler;
    namespace euler120 {
	u32 r_max(u16 const a){
	    u32 mod = a*a;
	    u32 const x = a-1;
	    u32 const y = a+1;
	    u32 xx = x;
	    u32 yy = y;
	    u32 max_r = 2;
	    do {
		u32 r = (xx+yy) % mod;
		if(r > max_r)
		    max_r = r;
		xx = (xx*x) % mod;
		yy = (yy*y) % mod;
	    } while(x != xx || y != yy);
	    return max_r;
	}
    }
    u32 problem120(){ 
	using namespace euler120;
	u32 ret = 0;
	for(u16 a = 3; a < 1001; ++a)
	    ret += r_max(a);
	return ret;
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set12
    {P(120)};
}
