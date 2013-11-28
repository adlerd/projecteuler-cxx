#include "set8.hh"
#include "util.hh"
#include <cassert>
#include <array>
#include <queue>

namespace euler {
    uint problem80(){
	uint psr = 2;
	uint psq = 4;
	uint sum = 0;
	bigint mult;
	mpz_ui_pow_ui(mult.get_mpz_t(), 100, 100);
	for(uint i = 2; i < 100; ++i){
	    if(i == psq){
		psr++;
		psq = psr*psr;
	    } else {
		bigint x = mult * i;
		x = sqrt(x);
		auto str = x.get_str();
		assert(str.size() >= 100);
		for(uint k = 0; k < 100; ++k)
		    sum += str[k] - '0';
	    }
	}
	return sum;
    }
    namespace eighty_dijkstra {
	std::array<uint, 80*80> const input81 = {{
#include <matrix.include>
	}};
	struct matloc {
	    uchar x;
	    uchar y;
	};
	struct place {
	    matloc xy;
	    uint value;
	    bool operator>(place const&) const;
	};
	bool place::operator>(place const& o) const {
	    /* For whatever reason, the C++ std lib uses a less-than comparison
	     * to implement a max-heap, and a greater-than comparison to
	     * implement a min-heap. */
	    if(value != o.value)
		return value > o.value;
	    if(xy.x != o.xy.x)
		return xy.x > o.xy.x;
	    return xy.y > o.xy.y;
	}
	typedef void (*neighborfun)(matloc, std::vector<matloc>&);
	auto locref(std::vector<uint>& v, matloc m) -> decltype(v[0]) {
	    assert((m.x < 80 && m.y < 80) || (m.x == 80 && m.y < 2));
	    return v[80*m.x+m.y];
	}
	uint dijkstra(neighborfun nborf){
	    std::priority_queue<place, std::vector<place>, std::greater<place>> pq;
	    std::vector<uint> vec(input81.cbegin(), input81.cend());
	    std::vector<matloc> nbors;
	    vec.push_back(1);
	    vec.push_back(1);
	    assert(vec.size() == 80*80+2);
	    pq.push({{80,0},0});
	    while(true){
		assert(!pq.empty());
		place top = pq.top();
		if(top.xy.x == 80 && top.xy.y == 1)
		    return top.value - 1;
		pq.pop();
		// The pq entry could be duplicate, because we don't delete it
		// when we find a better route
		if(locref(vec,top.xy) != 0) {
		    locref(vec,top.xy) = 0;
		    (*nborf)(top.xy, nbors);
		    for(matloc& m : nbors){
			uint v = locref(vec,m);
			if(v != 0)
			    pq.push({m,v + top.value});
		    }
		    nbors.clear();
		}
	    }
	}
	void problem81_neighbors(matloc m, std::vector<matloc>& v){
	    if(m.x < 79){
		v.push_back({(uchar)(m.x+1),m.y});
		if(m.y < 79)
		    v.push_back({m.x,(uchar)(m.y+1)});
	    } else if(m.x == 80){
		if(m.y == 0)
		    v.push_back({0,0});
	    } else if(m.x == 79 && m.y == 79){
		v.push_back({80,1});
	    } else {
		v.push_back({m.x,(uchar)(m.y+1)});
	    }
	}
	void problem82_neighbors(matloc m, std::vector<matloc>& v){
	    if(m.x == 80){
		if(m.y == 0){
		    for(int i = 0; i < 80; ++i)
			v.push_back({(uchar)i,0});
		}
	    } else if(m.y < 79){
		v.push_back({m.x,(uchar)(m.y+1)});
		if(m.x > 0)
		    v.push_back({(uchar)(m.x-1),m.y});
		if(m.x < 79)
		    v.push_back({(uchar)(m.x+1),m.y});
	    } else { // m.y == 79
		v.push_back({80,1});
	    }
	}
	void problem83_neighbors(matloc m, std::vector<matloc>& v){
	    if(m.x == 80){
		if(m.y == 0)
		    v.push_back({0,0});
	    } else if(m.x == 79 && m.y == 79) {
		v.push_back({80,1});
	    } else {
		if(m.x > 0)
		    v.push_back({(uchar)(m.x-1),m.y});
		if(m.x < 79)
		    v.push_back({(uchar)(m.x+1),m.y});
		if(m.y > 0)
		    v.push_back({m.x,(uchar)(m.y-1)});
		if(m.y < 79)
		    v.push_back({m.x,(uchar)(m.y+1)});
	    }
	}
    }
    uint problem81(){
	using namespace eighty_dijkstra;
	return dijkstra(&problem81_neighbors);
    }
    uint problem82(){
	using namespace eighty_dijkstra;
	return dijkstra(&problem82_neighbors);
    }
    uint problem83(){
	using namespace eighty_dijkstra;
	return dijkstra(&problem83_neighbors);
    }
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set8
    {{P(80),P(81),P(82),P(83)}};
}
