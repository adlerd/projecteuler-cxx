// Copyright 2014 David Adler

#include "util.hh"
#include "atkin.hh"
#include <cassert>
#include <array>
#include <queue>
#include <memory>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>

namespace {
    using namespace euler;
    u32 problem80(){
	u32 psr = 2;
	u32 psq = 4;
	u32 sum = 0;
	bigint mult;
	mpz_ui_pow_ui(mult.get_mpz_t(), 100, 100);
	for(u32 i = 2; i < 100; ++i){
	    if(i == psq){
		psr++;
		psq = psr*psr;
	    } else {
		bigint x = mult * i;
		x = sqrt(x);
		auto str = x.get_str();
		assert(str.size() >= 100);
		for(u32 k = 0; k < 100; ++k)
		    sum += str[k] - '0';
	    }
	}
	return sum;
    }
    namespace eighty_dijkstra {
	std::array<u32, 80*80> const input81 = {{
#include <matrix.include>
	}};
	struct matloc {
	    u8 x;
	    u8 y;
	};
	struct place {
	    matloc xy;
	    u32 value;
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
	auto locref(std::vector<u32>& v, matloc m) -> decltype(v[0]) {
	    assert((m.x < 80 && m.y < 80) || (m.x == 80 && m.y < 2));
	    return v[80*m.x+m.y];
	}
	u32 dijkstra(neighborfun nborf){
	    std::priority_queue<place, std::vector<place>, std::greater<place>> pq;
	    std::vector<u32> vec(input81.cbegin(), input81.cend());
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
			u32 v = locref(vec,m);
			if(v != 0)
			    pq.push({m,v + top.value});
		    }
		    nbors.clear();
		}
	    }
	}
	void problem81_neighbors(matloc m, std::vector<matloc>& v){
	    if(m.x < 79){
		v.push_back({(u8)(m.x+1),m.y});
		if(m.y < 79)
		    v.push_back({m.x,(u8)(m.y+1)});
	    } else if(m.x == 80){
		if(m.y == 0)
		    v.push_back({0,0});
	    } else if(m.x == 79 && m.y == 79){
		v.push_back({80,1});
	    } else {
		v.push_back({m.x,(u8)(m.y+1)});
	    }
	}
	void problem82_neighbors(matloc m, std::vector<matloc>& v){
	    if(m.x == 80){
		if(m.y == 0){
		    for(u8 i = 0; i < 80; ++i)
			v.push_back({i,0});
		}
	    } else if(m.y < 79){
		v.push_back({m.x,(u8)(m.y+1)});
		if(m.x > 0)
		    v.push_back({(u8)(m.x-1),m.y});
		if(m.x < 79)
		    v.push_back({(u8)(m.x+1),m.y});
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
		    v.push_back({(u8)(m.x-1),m.y});
		if(m.x < 79)
		    v.push_back({(u8)(m.x+1),m.y});
		if(m.y > 0)
		    v.push_back({m.x,(u8)(m.y-1)});
		if(m.y < 79)
		    v.push_back({m.x,(u8)(m.y+1)});
	    }
	}
    }
    u32 problem81(){
	using namespace eighty_dijkstra;
	return dijkstra(&problem81_neighbors);
    }
    u32 problem82(){
	using namespace eighty_dijkstra;
	return dijkstra(&problem82_neighbors);
    }
    u32 problem83(){
	using namespace eighty_dijkstra;
	return dijkstra(&problem83_neighbors);
    }
    namespace monopoly {
	u32 constexpr width = 40*3;
	u32 constexpr die_side_ct = 4;
	u32 constexpr roll_ways = die_side_ct * die_side_ct;
	typedef double val_t;
	typedef std::array<val_t,width> col_t; // each elt is a dest
	typedef std::array<col_t, width> mat_t; // each elt (column) is a src
	void mat_square_into(mat_t& into, mat_t const& from){
	    for(u32 i = 0; i < width; ++i){
		for(u32 j = 0; j < width; ++j){
		    val_t accum = 0;
		    for(u32 k = 0; k < width; ++k)
			accum += from[i][k] * from[k][j];
		    into[i][j] = accum;
		}
	    }
	}
	inline u32 modforty(u32 x){
	    /* divisionless simple modulo forty. of course, this introduces a
	     * branch... */
	    return (x < 40 ? x : x - 40);
	}
	struct state {
	    u8 square;
	    u8 doubles;
	    u32 to_index(){
		assert(square < 40 && doubles < 3);
		return square + 40 * doubles;
	    }
	    state with_roll(u32 val, bool dbl){
		state ret{(u8)modforty(val + square),
		          (u8)(dbl ? 1+doubles : 0)};
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
	    u8 next_rr;
	    u8 next_u;
	    u8 back_3;
	    bool trigger_cc;
	};
	void resolve_ch(col_t& vec, state dest, val_t chance, ch_vars vars){
	    val_t small = chance / 16;
	    u8 dbls = dest.doubles;
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
	    u8 sq = dest.square;
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
	std::vector<std::pair<u32, val_t>> s_rolls(){
	    std::vector<val_t> vec(2*die_side_ct);
	    for(u32 x = 1; x <= die_side_ct; ++x)
		for(u32 y = x+1; y <= die_side_ct; ++y)
		    vec[x+y] += 2;
	    decltype(s_rolls()) ret;
	    for(u32 i = 0; i < vec.size(); ++i){
		if(vec[i] != 0)
		    ret.push_back({i, vec[i] / roll_ways});
	    }
	    return ret;
	}
	std::vector<std::pair<u32, val_t>> run_alg(){
	    std::unique_ptr<mat_t> a(new mat_t);
	    for(u32 i = 0; i < width; ++i)
		for(u32 j = 0; j < width; ++j)
		    (*a)[i][j] = 0;
	    auto srolls = s_rolls();
	    // fill matrix for single state transition
	    for(u32 sq = 0; sq < 40; ++sq){
		for(u32 dbls = 0; dbls < 3; ++dbls){
		    state src{(u8)sq,(u8)dbls};
		    auto& col = (*a)[src.to_index()];
		    for(auto p : srolls)
			resolve(col,src.with_roll(p.first,false),p.second);
		    for(u32 d = 1; d <= die_side_ct; ++d)
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
	    for(u32 dest = 0; dest < 40; ++dest){
		val_t ch = 0;
		for(u32 src = 0; src < width; ++src){
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
	for(u8 i = 0; i < 3; ++i)
	    out << std::setfill('0') << std::setw(2) << result[i].first;
	return out.str();
    }
    namespace euler85 {
	struct soln {
	    u32 x;
	    u32 y;
	    i32 diff;
	    soln up() const {
		return soln{x,y+1,0};
	    }
	    soln left() const {
		return soln{x-1,y,0};
	    }
	    i32 get_diff(){
		if(diff == 0){
		    u32 val = x*(x+1)*y*(y+1)/4;
		    diff = ((i32)val)-2000000;
		}
		return diff;
	    }
	    u32 get_abs_diff(){
		i32 d = get_diff();
		return (u32)(d < 0 ? -d : d);
	    }
	    bool operator<(soln& other) {
		return get_abs_diff() < other.get_abs_diff();
	    }
	};
    }
    u32 problem85(){
	/* we want to find the first-quadrant lattice point closest to the
	 * hyperbola xy(x+1)(y+1)/4=2000000. (The LHS describing the number of
	 * subrectangles of a rectangle x-by-y.) In fact, by symmetry, we can
	 * limit ourselves to x in [1,2001], y in [1,x]. */
	using namespace euler85;
	soln min{2000,1,0};
	while(min.get_diff() >= 0)
	    min = min.left();
	for(soln lbnd = min; lbnd.x >= lbnd.y; lbnd = lbnd.left()){
	    soln next = lbnd.up();
	    while(next.get_diff() < 0){
		assert(next.get_diff() > lbnd.get_diff());
		lbnd = next;
		next = next.up();
	    }
	    /* now lbnd and next straddle the hyperbola. */
	    /* std::min and friends require constness... */
	    if(lbnd < min)
		min = lbnd;
	    if(next < min)
		min = next;
	}
	return min.x * min.y;
    }
    namespace euler86 {
	typedef std::unordered_map<u32,u32> imap_t;
	typedef std::pair<u32,u32> uu_t;
	u32 constexpr TARGET = 1000000;
	inline void update_solns(u32 ua, u32 ub, imap_t& imap){
	    i32 b = (i32) ub;
	    i32 s = ua < ub ? (i32)(ua + 1) : b;
	    s -= b;
	    s += b / 2;
	    if(s > 0)
		imap[ua] += s;
	}
    }
    u32 problem86(){
	using namespace euler86;
	pythag_iterator pi;
	imap_t imap;
	u32 running = 0;
	u32 last = 0;
	while(true){
	    pythag_iterator::triplet trip = *pi;
	    u32 const a = trip[0];
	    u32 const b = trip[1];
	    if(a != last){
		running += imap[last];
		if(running >= TARGET)
		    return last;
		imap.erase(last);
	    }
	    ++pi;
	    last = a;
	    update_solns(a,b,imap);
	    update_solns(b,a,imap);
	}
    }
    u32 problem87(){
	std::unordered_set<u32> set;
	u32 constexpr LIMIT = 50000000;
	prime_iterator ai;
	while(true){
	    u32 const ap = *ai++;
	    u32 const a = ap*ap*ap*ap;
	    if(a >= LIMIT)
		break;
	    prime_iterator bi;
	    while(true){
		u32 const bp = *bi++;
		u32 const b = a + bp*bp*bp;
		if(b >= LIMIT)
		    break;
		prime_iterator ci;
		while(true){
		    u32 const cp = *ci++;
		    u32 const c = b + (cp*cp);
		    if(c >= LIMIT)
			break;
		    set.insert(c);
		}
	    }
	}
	return set.size();
    }
    namespace euler88 {
	struct state {
	    u32 max_val;
	    u32 product;
	    u32 sum;
	    u32 len;
	    state append(u32 n) const {
		assert(n >= max_val);
		return {n,product*n,sum+n,len+1};
	    }
	    u32 kval() const {
		assert(product >= sum);
		return len + product - sum;
	    }
	};
	u32 constexpr limit = 12000;
	typedef std::array<u32, 11999> kmap;
	bool rec(state const st, kmap& map){
	    u32 k = st.kval();
	    if(k >= limit)
		return false;
	    if(k >= 2){
		u32 old = map[k-2];
		if(old == 0 || old > st.product)
		    map[k-2] = st.product;
	    }
	    u32 n = st.max_val;
	    while(rec(st.append(n++),map));
	    return true;
	}
    }
    u32 problem88(){
	using namespace euler88;
	kmap map{};
	for(u32 i = 2; i < limit; ++i)
	    rec({i,i,i,1},map);
	std::unordered_set<u32> set;
	u32 sum = 0;
	for(u32 i : map)
	    if(set.insert(i).second)
		sum += i;
	return sum;
    }
    std::array<std::string,1000> input89 = {
#include "roman.include"
};
    u32 problem89(){
	static std::array<std::pair<char,u32>,7> const digits =
	{{{'M', 1000},{'D',500},{'C',100},{'L',50},{'X',10},{'V',5},{'I',1}}};
	static std::array<std::pair<u8, u32>,13> const outs =
	{{{1, 1000},{2,900},{1,500},{2,400},{1,100},{2,90},
	     {1,50},{2,40},{1,10},{2,9},{1,5},{2,4},{1,1}}};
	u32 saved = 0;
	for(std::string in : input89){
	    u32 val = 0;
	    u32 prev = 1000;
	    for(char c : in){
		for(auto p : digits){
		    if(c == p.first){
			if(prev < p.second)
			    val -= 2*prev;
			val += p.second;
			prev = p.second;
			break;
		    }
		}
	    }
	    saved += in.length();
	    for(auto p : outs){
		while(val >= p.second){
		    saved -= p.first;
		    val -= p.second;
		}
	    }
	}
	return saved;
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set8
    {{P(80),P(81),P(82),P(83),P(84),P(85),P(86),P(87),P(88),P(89)}};
}
