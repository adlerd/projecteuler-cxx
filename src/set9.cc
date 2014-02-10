#include "set9.hh"
#include "util.hh"

#include <bitset>

namespace {
    using namespace euler;
    namespace euler90 {
	typedef std::bitset<9> cube;
	template <uint R> //R = left to add
	void rcubes(cube base, uint start, std::vector<cube>& out){
	    for(uint i = start; i < 11-R; ++i){
		cube c(base);
		c[i] = true;
		rcubes<R-1>(c,i+1,out);
	    }
	}
	template<>
	void rcubes<1>(cube base, uint start, std::vector<cube>& out){
	    for(uint i = start; i < 9; ++i){
		cube c(base);
		c[i] = true;
		out.push_back(c);
	    }
	    // simulate adding 9 to a cube, regardless of what's there already
	    cube c(base);
	    c[6] = true;
	    out.push_back(c);
	}
    }
    uint problem90(){
	using namespace euler90;
	std::vector<cube> cubes;
	rcubes<6>({},0,cubes);
	uint ct = 0;
	for(auto i1 = cubes.cbegin(); i1 != cubes.cend(); ++i1){
	    for(decltype(cubes)::const_iterator i2 = i1; i2 != cubes.cend(); ++i2){
		++ct;
		for(uint n = 1; n < 10; ++n){
		    uint sq = n*n;
		    uint d1 = sq / 10;
		    uint d2 = sq % 10;
		    if(d1 == 9)
			d1 = 6;
		    if(d2 == 9)
			d2 = 6;
		    if(!((i1->test(d1) && i2->test(d2)) ||
			    (i1->test(d2) && i2->test(d1)))){
			--ct;
			break;
		    }
		}
	    }
	}
	return ct;
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set9
    {{P(90)}};
}
