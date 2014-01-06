#include "set8.hh"
#include "util.hh"
#include <cassert>
#include <array>
#include <queue>
#include <memory>
#include <sstream>
#include <iomanip>

namespace {
    using namespace euler;
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
    namespace monopoly {
	uint constexpr width = 40*3;
	uint constexpr die_side_ct = 4;
	uint constexpr roll_ways = die_side_ct * die_side_ct;
	typedef double val_t;
	typedef std::array<val_t,width> col_t; // each elt is a dest
	typedef std::array<col_t, width> mat_t; // each elt (column) is a src
	void mat_square_into(mat_t& into, mat_t const& from){
	    for(uint i = 0; i < width; ++i){
		for(uint j = 0; j < width; ++j){
		    val_t accum = 0;
		    for(uint k = 0; k < width; ++k)
			accum += from[i][k] * from[k][j];
		    into[i][j] = accum;
		}
	    }
	}
	inline uint modforty(uint x){
	    /* divisionless simple modulo forty. of course, this introduces a
	     * branch... */
	    return (x < 40 ? x : x - 40);
	}
	struct state {
	    uchar square;
	    uchar doubles;
	    uint to_index(){
		assert(square < 40 && doubles < 3);
		return square + 40 * doubles;
	    }
	    state with_roll(uint val, bool dbl){
		state ret{(uchar)modforty(val + square),
		          (uchar)(dbl ? 1+doubles : 0)};
		if(ret.doubles > 2)
		    return {10,0};
		else
		    return ret;
	    }
	};
	void resolve_cc(col_t& vec, state dest, val_t chance){
	    val_t small = chance / 16;
	    vec[state{0,dest.doubles}.to_index()] += small;
	    vec[state{10,0}.to_index()] += small;
	    vec[dest.to_index()] += 14 * small;
	}
	struct ch_vars {
	    uchar next_rr;
	    uchar next_u;
	    uchar back_3;
	    bool trigger_cc;
	};
	void resolve_ch(col_t& vec, state dest, val_t chance, ch_vars vars){
	    val_t small = chance / 16;
	    uchar dbls = dest.doubles;
	    vec[state{0,dbls}.to_index()] += small;
	    vec[state{10,0}.to_index()] += small;
	    vec[state{11,dbls}.to_index()] += small;
	    vec[state{24,dbls}.to_index()] += small;
	    vec[state{39,dbls}.to_index()] += small;
	    vec[state{5,dbls}.to_index()] += small;
	    vec[state{vars.next_rr,dbls}.to_index()] += 2 * small;
	    vec[state{vars.next_u,dbls}.to_index()] += small;
	    vec[dest.to_index()] += 6 * small;
	    state target = state{vars.back_3,dbls};
	    if(vars.trigger_cc)
		resolve_cc(vec,target,small);
	    else
		vec[target.to_index()] += small;
	}
	void resolve(col_t& vec, state dest, val_t chance){
	    uchar sq = dest.square;
	    if(sq == 2 || sq == 18 || sq == 33){
		resolve_cc(vec,dest,chance);
	    } else if(sq == 7){
		resolve_ch(vec,dest,chance,{15,12,4,false});
	    } else if(sq == 22){
		resolve_ch(vec,dest,chance,{25,28,19,false});
	    } else if (sq == 36){
		resolve_ch(vec,dest,chance,{5,12,33,true});
	    } else if(sq == 30){
		vec[state{10,0}.to_index()] += chance;
	    } else {
		vec[dest.to_index()] += chance;
	    }
	}
	std::vector<std::pair<uint, val_t>> s_rolls(){
	    std::vector<val_t> vec(2*die_side_ct);
	    for(uint x = 1; x <= die_side_ct; ++x)
		for(uint y = x+1; y <= die_side_ct; ++y)
		    vec[x+y] += 2;
	    decltype(s_rolls()) ret;
	    for(uint i = 0; i < vec.size(); ++i){
		if(vec[i] != 0)
		    ret.push_back({i, vec[i] / roll_ways});
	    }
	    return ret;
	}
	std::vector<std::pair<uint, val_t>> run_alg(){
	    std::unique_ptr<mat_t> a(new mat_t);
	    for(uint i = 0; i < width; ++i)
		for(uint j = 0; j < width; ++j)
		    (*a)[i][j] = 0;
	    auto srolls = s_rolls();
	    // fill matrix for single state transition
	    for(uint sq = 0; sq < 40; ++sq){
		for(uint dbls = 0; dbls < 3; ++dbls){
		    state src{(uchar)sq,(uchar)dbls};
		    auto& col = (*a)[src.to_index()];
		    for(auto p : srolls)
			resolve(col,src.with_roll(p.first,false),p.second);
		    for(uint d = 1; d <= die_side_ct; ++d)
			resolve(col,src.with_roll(d*2,true),1.0/roll_ways);
		}
	    }
	    // run simulation 2^6 times
	    {
		std::unique_ptr<mat_t> b(new mat_t);
		mat_square_into(*b,*a);//2
		mat_square_into(*a,*b);//4
		mat_square_into(*b,*a);//8
		mat_square_into(*a,*b);//16
		mat_square_into(*b,*a);//32
		mat_square_into(*a,*b);//64
	    }
	    decltype(run_alg()) ret;
	    for(uint dest = 0; dest < 40; ++dest){
		val_t ch = 0;
		for(uint src = 0; src < width; ++src){
		    ch += (*a)[src][dest];
		    ch += (*a)[src][dest+40];
		    ch += (*a)[src][dest+80];
		}
		ret.emplace_back(dest,ch);
	    }
	    return ret;
	}
	typedef decltype(run_alg()[0]) result_item;
	bool comp_result(result_item& f, result_item& s){
	    return f.second > s.second;
	}
    }
    std::string problem84(){
	using namespace monopoly;
	auto result = run_alg();
	std::partial_sort(result.begin(), result.begin()+3,
		result.end(),&comp_result);
	std::ostringstream out;
	for(int i = 0; i < 3; ++i)
	    out << std::setfill('0') << std::setw(2) << result[i].first;
	return out.str();
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set8
    {{P(80),P(81),P(82),P(83),P(84)}};
}
