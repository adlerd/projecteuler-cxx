// Copyright 2014 David Adler

#include "util.hh"

#include <vector>
#include <array>
#include <forward_list>
#include <bitset>
#include <sstream>

namespace {
    using namespace euler;
    bigint problem100(){
	u64 a0 = 1;
	u64 a1 = 4;
	u64 a2 = 21;
	while(true){
	    // 7*a(n-1)-7*a(n-2)+a(n-3)
	    u64 an = 7*(a2 - a1) + a0;
	    if(an > 1000000000000){
		bigint v = an;
		v = v*(v-1) * 2 + 1;
		mpz_sqrt(v.get_mpz_t(),v.get_mpz_t());
		return (v + 1) / 2;
	    }
	    a0 = a1;
	    a1 = a2;
	    a2 = an;
	}
    }
    namespace euler101 {
	typedef std::vector<i64> polynomial;
	i64 eval_poly(polynomial const& p, i64 x){
	    i64 xx = 1;
	    i64 ret = 0;
	    for(i64 n = 0; n < p.size(); ++n){
		ret += xx*p[n];
		xx *= x;
	    }
	    return ret;
	}
	polynomial fit_poly(std::vector<i64> ret){ // input starts at x=1!
	    i32 const size = ret.size();
	    std::vector<i64> matrix(size*size);
	    /* together matrix and ret form the augmented matrix */
	    auto iter = matrix.begin();
	    for(i64 x = 1; x <= size; ++x){
		i64 xx = 1;
		for(u32 n = 0; n < size; ++n){
		    *iter++ = xx;
		    xx *= x;
		}
	    }
	    assert(iter == matrix.end());
	    for(u32 r0 = 0; r0 < size; ++r0){
		i64 const v0 = matrix[r0*size + r0];
		for(u32 r = r0+1; r < size; ++r){
		    i64 const v1 = matrix[r*size + r0];
		    if(v1 == 0)
			continue;
		    i64 const g = (i64) gcd(v0 < 0 ? -v0 : v0,v1 < 0 ? -v1 : v1);
		    i64 const m0 = v0 / g;
		    i64 const m1 = v1 / g;
		    for(u32 c = r0; c < size; ++c)
			matrix[r*size+c] = matrix[r*size+c] * m0 -
			    matrix[r0*size+c] * m1;
		    ret[r] = ret[r] * m0 - ret[r0] * m1;
		    assert(matrix[r*size + r0] == 0);
		}
	    }
	    for(i64 r = size-1; r >= 0; --r){
		for(u32 c = r+1; c < size; ++c)
		    ret[r] -= matrix[r*size+c] * ret[c];
		if(ret[r] % matrix[r*size + r] != 0)
		    throw std::logic_error("non-integral polynomial fit");
		ret[r] /= matrix[r*size + r];
	    }
	    return ret;
	}
    }
    i64 problem101(){
	using namespace euler101;
	polynomial const poly{{1,-1,1,-1,1,-1,1,-1,1,-1,1}};
	std::vector<i64> eval;
	i64 sum = 0;
	for(i64 x = 1; x < 11; ++x){
	    eval.push_back(eval_poly(poly,x));
	    polynomial const approx = fit_poly(eval);
	    i64 const fit = eval_poly(approx,x+1);
	    sum += fit;
	}
	return sum;
    }
    std::array<i32,6000> const input102{{
#include "triangles.include"
    }};
    u32 problem102(){
	u32 ct = 0;
	for(u32 i = 0; i < 1000; ++i){
	    i32 const *const tri = input102.cbegin()+6*i;
	    i32 const ax = tri[0], ay = tri[1];
	    i32 const bx_a = tri[2] - ax, by_a = tri[3] - ay;
	    i32 const cx_a = tri[4] - ax, cy_a = tri[5] - ay;
	    // find the coordinates of the origin in a coordinate system
	    // with (ax,ay) as the origin and bx,by cx,cy as basis vectors
	    // the coordinates are ox_d/d, oy_d/d.
	    // To do this, multiply the inverse of the matrix
	    // {{cx_a,bx_a},{cy_a,by_a}} by {-ax,-ay}
	    i32 const d = by_a*cx_a-bx_a*cy_a; // determinant of matrix
	    i32 const ox_d = -ax*by_a +ay*bx_a; // d*first elt of origin vec
	    i32 const oy_d = ax*cy_a -ay*cx_a; // d*second elt of origin vec
	    // finally, test ox_d/d > 0, oy_d/d > 0, ox_d/d+oy_d/d<1
	    // don't use short-circuiting for efficiency
	    if(d > 0 ? (ox_d > 0 & oy_d > 0 & ox_d+oy_d < d) :
		    (ox_d < 0 & oy_d < 0 & ox_d+oy_d > d))
		++ct;
	}
	return ct;
    }
    namespace euler103 {
	/* generate possible subsets of given set size, ordered by pop ct */
	std::vector<u16> gen_subs(u32 const size) noexcept {
	    std::vector<std::forward_list<u16>> stacks(size-1);
	    u32 const xsize = 1 << size;
	    for(u16 i = 1; i < xsize - 1; ++i){
		u64 const pop_ct = std::bitset<16>(i).count();
		stacks[pop_ct - 1].push_front(i);
	    }
	    std::vector<u16> ret(xsize - 2 + size - 1);
	    auto iter = ret.begin();
	    auto prev = iter;
	    for(auto const& stack : stacks){
		for(u16 i : stack)
		    *iter++ = i;
		std::sort(prev, iter);
		*iter++ = 0;
		prev = iter;
	    }
	    assert(iter == ret.end());
	    return ret;
	}
	bool dominates(u16 const a, u16 const b, u32 const size){
	    u32 count = 0;
	    u16 mask = 1 << (size - 1);
	    while(mask){
		if(mask & a)
		    ++count;
		if(mask & b){
		    if(count == 0)
			return false;
		    --count;
		}
		mask >>= 1;
	    }
	    return true;
	}
	/* bitcode: each instruction is a u16.
	 * set "left sum" from subset:
	 * MSB[0:pad][sum left:size][1:2]LSB
	 * check subset sum >/!= "left sum":
	 * MSB[0:pad][sum right:size][ordered:1][0:1]LSB
	 */
	std::vector<u16> gen_check_bitcode(u32 const size){
	    auto const subs = gen_subs(size);
	    auto iter0 = subs.cbegin();
	    std::vector<u16> ret;
	    while(true){
		if(*iter0 == 0){
		    ++iter0;
		    if(iter0 == subs.cend())
			return ret;
		}
		u16 const left = *iter0++;
		bool need_left = true;
		bool ordered = false;
		auto iter1 = iter0;
		while(true){
		    if(*iter1 == 0){
			ordered = true;
			++iter1;
			if(iter1 == subs.cend())
			    break;
		    }
		    u16 const right = *iter1++;
		    if((left & right) == 0 && !dominates(right,left,size)){
			if(need_left){
			    ret.push_back((left << 2) | 0x1);
			    need_left = false;
			}
			ret.push_back((right << 2) | (ordered ? 0x2 : 0x0));
		    }
		}
	    }
	}
#if 0
	std::string dump_bitcode(std::vector<u16> const& code, u32 const size){
	    std::ostringstream ret_out;
	    std::ostringstream left_out;
	    ret_out << "SIZE = " << size;
	    for(u16 elt : code){
		std::ostringstream *out;
		if(elt & 0x1){
		    out = &left_out;
		    left_out.str("");
		} else {
		    out = &ret_out;
		    ret_out << std::endl << left_out.str();
		    if(elt & 0x2)
			ret_out << "\t<  ";
		    else
			ret_out << "\t!= ";
		}
		elt >>= 2;
		for(char c = 'a'; c < 'a' + size; ++c){
		    if(elt & 0x1)
			*out << c;
		    elt >>= 1;
		}
	    }
	    ret_out << std::endl;
	    return ret_out.str();
	}
#endif
	bool check_sss(std::vector<u16> const& code, std::vector<u32> const&
		vals){
	    assert(std::is_sorted(vals.cbegin(), vals.cend()));
	    u32 sum_left = 0;
	    for(u16 const instr : code){
		u32 sum = 0;
		u16 sub = instr >> 2;
		auto val_iter = vals.cbegin();
		while(sub != 0){
		    assert(val_iter != vals.cend());
		    if(sub & 0x1)
			sum += *val_iter;
		    ++val_iter;
		    sub >>= 1;
		}
		switch(instr & 0x3){
		case 0:
		    if(sum == sum_left)
			return false;
		    break;
		case 2:
		    if(sum <= sum_left)
			return false;
		    break;
		default:
		    sum_left = sum;
		}
		assert(sum_left); // first instr should set
	    }
	    return true;
	}
	struct state103 {
	    u32 best_bnd;
	    u32 min;
	    std::vector<u32> best;
	    std::vector<u32> current;
	    std::vector<u16> const code;
	    state103(u32 size, u32 m, u32 bnd)
		: best_bnd(bnd), min(m), best(size), current(size),
		  code(gen_check_bitcode(size)) {}
	};
	void level103(u32 const level, u32 const psum, state103& state){
	    u32 size = state.current.size();
	    if(level == size){
		if(psum <= state.best_bnd && check_sss(state.code,
			    state.current)){
		    state.best_bnd = psum;
		    state.best = state.current;
		}
	    } else {
		u32 n = level == 0 ? state.min : state.current[level-1]+1;
		while((size - level)*n+psum <= state.best_bnd){
		    state.current[level] = n;
		    level103(level+1, psum+n, state);
		    ++n;
		}
	    }
	}
	std::array<std::vector<u32>,100> const input105 = {{
#include <subsets.include>
	}};
    }
    std::string problem103(){
	using namespace euler103;
	state103 state(7, 20, 115 + 7*20);
	level103(0, 0, state);
	std::ostringstream out;
	for(u32 v : state.best)
	    out << v;
	return out.str();
    }
    namespace euler104 {
	u64 constexpr tennine = (u64)(100000)*(u64)(100000) / 10;
	bool pandigital(u64 b){
	    if(b < tennine/10 || b >= tennine)
		return false;
	    digit_iterator di(b);
	    std::bitset<9> found;
	    while(di != digit_iterator(0)){
		u8 const d = *di;
		if(d == 0)
		    return false;
		if(found[d-1])
		    return false;
		found[d-1] = true;
		++di;
	    }
	    return true;
	}
    }
    u32 problem104(){
	using namespace euler104;
	bigint a = 0;
	bigint b = 1;
	bigint ten_den = tennine;
	bigint c;
	u32 n = 1;
	while(true){
	    mpz_swap(a.get_mpz_t(), b.get_mpz_t());
	    b += a;
	    ++n;
	    if(!pandigital(mpz_fdiv_ui(b.get_mpz_t(), tennine)))
		continue;
	    c = b / ten_den;
	    while(c > tennine){
		c /= 10;
		ten_den *= 10;
	    }
	    if(pandigital(c.get_ui()))
		return n;
	}
    }
    u64 problem105(){
	using namespace euler103;
	std::vector<std::vector<u16>> code_store(6);
	for(u32 size = 7; size <= 12; ++size)
	    code_store[size - 7] = gen_check_bitcode(size);
	u64 sum = 0;
	for(auto vals : input105){ // copy vector!
	    std::sort(vals.begin(), vals.end());
	    u32 const size = vals.size();
	    assert(size >= 7 && size <= 12);
	    std::vector<u16> const& code = code_store[size - 7];
	    if(check_sss(code, vals))
		for(u32 v : vals)
		    sum += v;
	}
	return sum;
    }
    u32 problem106(){
	using namespace euler103;
	auto const code = gen_check_bitcode(12);
	u32 ct = 0;
	for(u16 instr : code)
	    if((instr & 0x3) == 0)
		++ct;
	return ct;
    }
    namespace euler107 {
	u16 constexpr SIZE = 40;
	std::array<u16, SIZE*SIZE> input107 = {{
#include<network.include>
	}};
	struct edge {
	    u16 weight;
	    u16 connects;
	    edge(u16 w, u16 c) : weight(w), connects(c) {}
	    bool operator<(edge const e) const {
		if(weight != e.weight)
		    return weight > e.weight;
		else
		    return connects > e.connects;
	    }
	};
    }
    u32 problem107(){
	using namespace euler107;
	u32 weight = 0;
	for(u32 i = 0; i < SIZE; ++i)
	    for(u32 j = 0; j <= i; ++j)
		weight += input107[i*SIZE + j];
	std::bitset<SIZE> connected;
	std::priority_queue<edge> pq;
	pq.emplace(0,0);
	do {
	    edge top = pq.top();
	    pq.pop();
	    if(!connected[top.connects]) {
		weight -= top.weight;
		connected[top.connects] = true;
		u32 const base = SIZE * top.connects;
		for(u32 i = 0; i < SIZE; ++i)
		    if(input107[base + i] != 0)
			pq.emplace(input107[base + i], i);
	    }
	} while(!pq.empty());
	assert(connected.all());
	return weight;
    }
    namespace euler108 {
	inline u32 recipSolns(u64 n){
	    if(n >> 32 != 0)
		throw std::domain_error("recipSolns will overflow");
	    return (1+divisor_ct(n*n))/2;
	}
    }
    u32 problem108(){
	using namespace euler108;
	u32 n = 0;
	while(recipSolns(++n) < 1000);
	return n;
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set10
    {P(100),P(101),P(102),P(103),P(104),P(105),P(106),P(107),P(108)};
}
