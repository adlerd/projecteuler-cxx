// Copyright 2014 David Adler

#include "util.hh"
#include "algx.hh"

#include <bitset>
#include <forward_list>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>
#include <cmath>

namespace {
    using namespace euler;
    u32 problem90(){
	typedef std::bitset<9> cube;
	std::vector<cube> cubes;
	std::array<u8,10> nums{{0,1,2,3,4,5,6,7,8,9}};
	do {
	    cube cube;
	    for(u32 i = 0; i < 6; ++i){
		u32 n = nums[i];
		if(n == 9)
		    n = 6;
		cube[n] =true;
	    }
	    cubes.push_back(cube);
	} while(next_rcombination(nums.begin(), nums.begin()+6, nums.end()));
	u32 ct = 0;
	for(auto i1 = cubes.cbegin(); i1 != cubes.cend(); ++i1){
	    for(decltype(cubes)::const_iterator i2 = i1; i2 != cubes.cend(); ++i2){
		++ct;
		for(u32 n = 1; n < 10; ++n){
		    u32 sq = n*n;
		    u32 d1 = sq / 10;
		    u32 d2 = sq % 10;
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
    u32 problem91(){
	/* There are three cases: (1) the right angle is at the origin, (2) the
	 * right angle is at the "greater" of (P,Q), and (3) the right angle is
	 * at the "lesser" of (P,Q) (for some fixed ordering of points).
	 * Case 1: P and Q will be on each axis. Case 2: These can be
	 * enumerated for each possible P (where P is the greater of the two
	 * points), and enumerating the points on the line from the angle
	 * "downwards". Case 3: There is a bijection between these and Case 2,
	 * which cases are disjoint. */
	u32 constexpr LIMIT = 50;
	u32 ct = LIMIT * LIMIT; // Case 1: 50C1 * 50C1
	for(u32 px = 0; px <= LIMIT; ++px){
	    for(u32 py = 1; py <= LIMIT; ++py){
		// Find the gcd of px and py and use these as increments
		u32 g = gcd(px,py);
		u32 gx = py / g; // (x,y) swapped
		u32 gy = px / g;
		u32 qx = px;
		u32 qy = py;
		while(qx <= (LIMIT - gx) && qy >= gy){
		    // we count the "next" iteration to avoid unsigned 0 issues
		    ct += 2;
		    qx += gx;
		    qy -= gy;
		}
	    }
	}
	return ct;
    }
    u32 problem92(){
	u32 constexpr LIMIT = 10000000;
	std::vector<bool> bits(2*LIMIT); // pairs: (valid, 89)
	bits[2*1] = true;
	bits[2*89] = true;
	bits[2*89+1] = true;
	u32 ct = 0;
	for(u32 i = 1; i < LIMIT; ++i){
	    if(!bits[2*i]){
		std::forward_list<u32> stack;
		u32 n = i;
		while(true){
		    if(n < LIMIT){
			if(bits[2*n])
			    break;
			bits[2*n] = true;
			/* we're guaranteed that we won't hit n in a loop
			 * again, so go ahead and set valid here so if the loop
			 * hits 1, we don't even have to iterate through stack
			 * to set valid bits */
			stack.push_front(n);
		    }
		    u32 next = 0;
		    for(digit_iterator di(n); di != digit_iterator(); ++di){
			u8 const d = *di;
			next += d * d;
		    }
		    n = next;
		}
		if(bits[2*n+1]){
		    for(u32 nn : stack)
			bits[2*nn + 1] = true;
		}
	    }
	    if(bits[2*i+1])
		++ct;
	}
	return ct;
    }
    namespace euler93 {
	struct small;
	typedef small (*op)(small const, u8 const);
	struct small {
	    /* always in lowest terms; d=0 indicates invalid */
	    i16 n;
	    i16 d;
	    typedef decltype(n*d) extended;
	    small(extended nn, extended dd) : n(nn), d(dd) {}
	    void normalize(){
		if(d == 0){
		    n = 0;
		    return;
		}
		bool sgn = false;
		if(d < 0){
		    sgn = !sgn;
		    d = -d;
		}
		if(n < 0){
		    sgn = !sgn;
		    n = -n;
		}
		u64 g = gcd(n,d);
		if(sgn)
		    n = -n;
		if(g == 1)
		    return;
		n /= g;
		d /= g;
	    }
	    small with_op(op const f, u8 const x) const {
		small ret(f(*this,x));
		ret.normalize();
		return ret;
	    }
	    bool operator<(small const other) const {
		if(n != other.n)
		    return n < other.n;
		else
		    return d < other.d;
	    }
	};
	small mult(small const a, u8 const x){
	    return {a.n * x, a.d};
	}
	small div(small const a, u8 const x){
	    return {a.n, a.d * x};
	}
	small flipdiv(small const a, u8 const x){
	    // if a is in normal form, a.d == 0 -> a.n == 0
	    return {a.d * x, a.n};
	}
	small plus(small const a, u8 const x){
	    return {a.n + x * a.d, a.d};
	}
	small minus(small const a, u8 const x){
	    return {a.n - x * a.d, a.d};
	}
	small flipminus(small const a, u8 const x){
	    return {x * a.d - a.n, a.d};
	}
	std::array<op, 6> constexpr ops{{&div, &flipdiv, &mult, &plus, &minus,
	    &flipminus}};
    }
    u32 problem93(){
	using namespace euler93;
	std::array<u8,9> digs{{1,2,3,4,5,6,7,8,9}};
	u32 maxval = 0;
	u32 maxtag = 0;
	do {
	    std::set<u32> res;
	    do {
		small const s0{digs[0],1};
		for(auto o1 : ops){
		    small const s1 = s0.with_op(o1,digs[1]);
		    for(auto o2 : ops){
			small const s2 = s1.with_op(o2,digs[2]);
			if(s2.d == 0)
			    continue;
			for(auto o3 : ops){
			    small const s3 = s2.with_op(o3,digs[3]);
			    if(s3.d == 1 && s3.n > 0)
				res.insert(s3.n);
			}
		    }
		}
	    } while(std::next_permutation(digs.begin(), digs.begin()+4));
	    u32 n = 1;
	    for(u32 s : res){
		if(s != n)
		    break;
		else
		    ++n;
	    }
	    if(n > maxval){
		maxval = n;
		maxtag = 1000*digs[0] + 100*digs[1] + 10*digs[2] + digs[3];
	    }
	} while(next_rcombination(digs.begin(), digs.begin()+4, digs.end()));
	return maxtag;
    }
    u32 problem94(){
	u32 constexpr LIMIT = 1000000000;
	u32 l0 = 1;
	u32 l1 = 1;
	bool up = true;
	u64 sum = 0;
	while(true){
	    // generate the sequence
	    u32 ab = 4*l1 - l0;
	    if(up)
		ab += 2;
	    else
		ab -= 2;
	    up = !up;
	    l0 = l1;
	    l1 = ab;
	    // use the value
	    for(u32 c : {ab-1,ab+1}){
		u64 const s = (u64) ab + c / 2;
		u64 const add = 2*s;
		if(add > LIMIT)
		    return sum;
		if(isqrt(s*(s-c)) != 0)
		    sum += add;
	    }
	}
    }
    u32 problem95(){
	u32 constexpr LIMIT = 1000000;
	std::vector<bool> hit(LIMIT+1);
	hit[0] = true;
	hit[1] = true;
	u32 maxlen = 0;
	u32 maxmin = 0;
	/* i only matters as the starting point for our loop-search to ensure
	 * we search everywhere; the found loop might not contain i if it was
	 * on a 'tail' */
	for(u32 i = 2; i <= LIMIT; ++i){
	    if(hit[i])
		continue;
	    std::forward_list<u32> stack;
	    u32 found = 0;
	    for(u32 n = i; n <= LIMIT; n = divisor_sum(n) - n){
		if(hit[n]){
		    found = n;
		    break;
		}
		stack.push_front(n);
		hit[n] = true;
	    }
	    if(found != 0){
		u32 min = found;
		u32 ct = 0;
		/* if we're in a loop, it contains found */
		for(u32 n : stack){
		    if(n < min)
			min = n;
		    ++ct;
		    if(n == found){
			/* then we've found a new loop, with min elt. min and
			 * length ct */
			if(ct > maxlen){
			    maxlen = ct;
			    maxmin = min;
			}
			break;
		    }
		}
	    }
	}
	return maxmin;
    }
    namespace euler96 {
	//sudoku constraint
	inline u32 scon(u8 unit, u8 val){
	    return 9*unit + val;
	}
	inline u32 scon(u8 row, u8 col, u8 val){
	    return scon(9*row+col, val);
	}
	inline u32 scon(u8 box_row, u8 box_col, u8 in_row, u8 in_col, u8 val){
	    return scon(3*box_row + in_row, 3*box_col + in_col, val);
	}
	void sudoku_base(algx_state& st){
	    for(u32 s_val = 0; s_val < 9; ++s_val){
		// val unique in row
		for(u32 s_row = 0; s_row < 9; ++s_row){
		    st.push_required();
		    for(u32 s_col = 0; s_col < 9; ++s_col)
			st.add_required_entry(scon(s_row,s_col,s_val));
		}
		// val unique in col
		for(u32 s_col = 0; s_col < 9; ++s_col){
		    st.push_required();
		    for(u32 s_row = 0; s_row < 9; ++s_row)
			st.add_required_entry(scon(s_row,s_col,s_val));
		}
		// val unique in box
		for(u32 s_box_r = 0; s_box_r < 3; ++s_box_r){
		    for(u32 s_box_c = 0; s_box_c < 3; ++s_box_c){
			st.push_required();
			for(u32 s_idx_r = 0; s_idx_r < 3; ++s_idx_r)
			    for(u32 s_idx_c = 0; s_idx_c < 3; ++s_idx_c)
				st.add_required_entry(scon(s_box_r, s_box_c,
					    s_idx_r, s_idx_c, s_val));
		    }
		}
	    }
	    // one val per unit
	    for(u32 s_unit = 0; s_unit < 81; ++s_unit){
		st.push_required();
		for(u32 s_val = 0; s_val < 9; ++s_val)
		    st.add_required_entry(scon(s_unit, s_val));
	    }
	}
	std::array<char const *const, 50> input96 = {{
#include "sudoku.include"
	}};
    }
    u32 problem96(){
	using namespace euler96;
	algx_state st(81*9);
	sudoku_base(st);
	u32 ct = 0;
	u32 sum = 0;
	for(char const *c : input96){
	    st.clean();
	    // clear old singletons
	    while(ct > 0){
		st.pop_required();
		--ct;
	    }
	    // singleton constraints at already-filled values
	    for(u32 s_unit = 0; s_unit < 81; ++s_unit){
		if(*c != '0'){
		    ++ct;
		    st.push_required();
		    st.add_required_entry(scon(s_unit, *c-'1'));
		}
		++c;
	    }
	    assert(*c == '\0');
	    if(!st.next_solution()) {
		return 0;
	    } else {
		std::vector<u32> s = st.read_solution();
		/* the row indices are computed with highest base-9 digits as
		 * row, then as column, then value, so sorting them gives
		 * row-major sort of units, and the bottom mod-9 segment gives
		 * the value */
		std::partial_sort(s.begin(),s.begin()+3,s.end());
		sum += 100*(s[0]%9+1);
		sum += 10*(s[1]%9+1);
		sum += 1*(s[2]%9+1);
	    }
	}
	return sum;
    }
    bigint problem97(){
	bigint mod;
	mpz_ui_pow_ui(mod.get_mpz_t(), 10, 10);
	bigint b(2);
	mpz_powm_ui(b.get_mpz_t(), b.get_mpz_t(), 7830457, mod.get_mpz_t());
	b *= 28433;
	b += 1;
	b %= mod;
	return b;
    }
    namespace euler98 {
	typedef std::vector<u8> arrangement;
	typedef std::array<u8,26> anagram_class;
	typedef std::map<anagram_class, std::list<arrangement>> anagram_map;
	u64 largest_square(std::list<arrangement> const& list){
	    if(list.size() < 2)
		return 0;
	    u32 const n = 1 + *std::max_element(list.front().cbegin(),
		    list.front().cend());
	    if(n > 9)
		return 0;
	    std::array<u8, 9> digs = {{1,2,3,4,5,6,7,8,9}};
	    std::vector<u8> out(list.front().size());
	    u64 max = 0;
	    do {
		do {
		    u64 first = 0;
		    bool two = false;
		    for(auto& arr : list) {
			assert(arr.size() == out.size());
			auto ai = arr.cbegin();
			auto oi = out.begin();
			while(ai != arr.cend())
			    *oi++ = digs.at(*ai++);
			u64 val = from_digits(out.cbegin(), out.cend());
			if(two){
			    if(val > max && isqrt(val) != 0)
				max = val;
			} else if(isqrt(val) != 0){
			    if(first == 0) {
				first = val;
			    } else {
				two = true;
				max = std::max({val,first,max});
			    }
			}
		    }
		} while(std::next_permutation(digs.begin(), digs.begin()+n));
	    } while(next_rcombination(digs.begin(), digs.begin()+n,
			digs.end()));
	    return max;
	}
	void classify_anagram(std::string const& str, anagram_map& into){
	    anagram_class cl = {};
	    for(char c : str)
		cl[c-'A']++;
	    std::map<char, u32> indices;
	    u32 x = 0;
	    for(u32 c = 0; c < 26; ++c)
		if(cl[c] != 0)
		    indices[c+'A'] = x++;
	    arrangement arr;
	    for(char c : str)
		arr.push_back(indices[c]);
	    into[cl].push_back(arr);
	}
    }
    u64 problem98(){
	using namespace euler98;
	anagram_map map;
	for(std::string const& str : input_words)
	    classify_anagram(str, map);
	u64 max = 0;
	for(auto const& p : map){
	    u64 s = largest_square(p.second);
	    if(s > max)
		max = s;
	}
	return max;
    }
    std::array<u32, 2000> input99 = {{
#include "base_exp.include"
    }};
    u64 problem99(){
	auto iter = input99.cbegin();
	u32 n = 1;
	u32 maxn = 0;
	double maxv = 0.0;
	while(iter != input99.cend()){
	    double const b = *iter++;
	    double const e = *iter++;
	    double const v = e * log(b);
	    if(v > maxv){
		maxv = v;
		maxn = n;
	    }
	    ++n;
	}
	return maxn;
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set9
    {{P(90),P(91),P(92),P(93),P(94),P(95),P(96),P(97),P(98),P(99)}};
}
