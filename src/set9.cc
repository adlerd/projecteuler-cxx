#include "set9.hh"
#include "util.hh"
#include "algx.hh"

#include <bitset>
#include <forward_list>
#include <set>
#include <algorithm>
#include <cassert>

namespace {
    using namespace euler;
    uint problem90(){
	typedef std::bitset<9> cube;
	std::vector<cube> cubes;
	std::array<uchar,10> nums{{0,1,2,3,4,5,6,7,8,9}};
	do {
	    cube cube;
	    for(uint i = 0; i < 6; ++i){
		uint n = nums[i];
		if(n == 9)
		    n = 6;
		cube[n] =true;
	    }
	    cubes.push_back(cube);
	} while(next_rcombination(nums.begin(), nums.begin()+6, nums.end()));
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
    uint problem91(){
	/* There are three cases: (1) the right angle is at the origin, (2) the
	 * right angle is at the "greater" of (P,Q), and (3) the right angle is
	 * at the "lesser" of (P,Q) (for some fixed ordering of points).
	 * Case 1: P and Q will be on each axis. Case 2: These can be
	 * enumerated for each possible P (where P is the greater of the two
	 * points), and enumerating the points on the line from the angle
	 * "downwards". Case 3: There is a bijection between these and Case 2,
	 * which cases are disjoint. */
	uint constexpr LIMIT = 50;
	uint ct = LIMIT * LIMIT; // Case 1: 50C1 * 50C1
	for(uint px = 0; px <= LIMIT; ++px){
	    for(uint py = 1; py <= LIMIT; ++py){
		// Find the gcd of px and py and use these as increments
		uint g = gcd(px,py);
		uint gx = py / g; // (x,y) swapped
		uint gy = px / g;
		uint qx = px;
		uint qy = py;
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
    uint problem92(){
	uint constexpr LIMIT = 10000000;
	std::vector<bool> bits(2*LIMIT); // pairs: (valid, 89)
	bits[2*1] = true;
	bits[2*89] = true;
	bits[2*89+1] = true;
	uint ct = 0;
	for(uint i = 1; i < LIMIT; ++i){
	    if(!bits[2*i]){
		std::forward_list<uint> stack;
		uint n = i;
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
		    uint next = 0;
		    for(digit_iterator di(n); di != digit_iterator(); ++di){
			uchar const d = *di;
			next += d * d;
		    }
		    n = next;
		}
		if(bits[2*n+1]){
		    for(uint nn : stack)
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
	typedef small (*op)(small const, uchar const);
	struct small {
	    /* always in lowest terms; d=0 indicates invalid */
	    int16_t n;
	    int16_t d;
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
		ulong g = gcd(n,d);
		if(sgn)
		    n = -n;
		if(g == 1)
		    return;
		n /= g;
		d /= g;
	    }
	    small with_op(op const f, uchar const x) const {
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
	small mult(small const a, uchar const x){
	    return {a.n * x, a.d};
	}
	small div(small const a, uchar const x){
	    return {a.n, a.d * x};
	}
	small flipdiv(small const a, uchar const x){
	    // if a is in normal form, a.d == 0 -> a.n == 0
	    return {a.d * x, a.n};
	}
	small plus(small const a, uchar const x){
	    return {a.n + x * a.d, a.d};
	}
	small minus(small const a, uchar const x){
	    return {a.n - x * a.d, a.d};
	}
	small flipminus(small const a, uchar const x){
	    return {x * a.d - a.n, a.d};
	}
	std::array<op, 6> constexpr ops{{&div, &flipdiv, &mult, &plus, &minus,
	    &flipminus}};
    }
    uint problem93(){
	using namespace euler93;
	std::array<uchar,9> digs{{1,2,3,4,5,6,7,8,9}};
	uint maxval = 0;
	uint maxtag = 0;
	do {
	    std::set<uint> res;
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
	    uint n = 1;
	    for(uint s : res){
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
    uint problem94(){
	uint constexpr LIMIT = 1000000000;
	uint l0 = 1;
	uint l1 = 1;
	bool up = true;
	ulong sum = 0;
	while(true){
	    // generate the sequence
	    uint ab = 4*l1 - l0;
	    if(up)
		ab += 2;
	    else
		ab -= 2;
	    up = !up;
	    l0 = l1;
	    l1 = ab;
	    // use the value
	    for(uint c : {ab-1,ab+1}){
		ulong const s = (ulong) ab + c / 2;
		ulong const add = 2*s;
		if(add > LIMIT)
		    return sum;
		if(isqrt(s*(s-c)) != 0)
		    sum += add;
	    }
	}
    }
    uint problem95(){
	uint constexpr LIMIT = 1000000;
	std::vector<bool> hit(LIMIT+1);
	hit[0] = true;
	hit[1] = true;
	uint maxlen = 0;
	uint maxmin = 0;
	/* i only matters as the starting point for our loop-search to ensure
	 * we search everywhere; the found loop might not contain i if it was
	 * on a 'tail' */
	for(uint i = 2; i <= LIMIT; ++i){
	    if(hit[i])
		continue;
	    std::forward_list<uint> stack;
	    uint found = 0;
	    for(uint n = i; n <= LIMIT; n = divisor_sum(n) - n){
		if(hit[n]){
		    found = n;
		    break;
		}
		stack.push_front(n);
		hit[n] = true;
	    }
	    if(found != 0){
		uint min = found;
		uint ct = 0;
		/* if we're in a loop, it contains found */
		for(uint n : stack){
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
	void sudoku_base(algx_state& st){
	    for(uint s_val = 0; s_val < 9; ++s_val){
		for(uint s_row = 0; s_row < 9; ++s_row){
		    st.push_required();
		    for(uint s_col = 0; s_col < 9; ++s_col)
			st.add_required_entry(81*s_row + 9*s_col + s_val);
		}
		for(uint s_col = 0; s_col < 9; ++s_col){
		    st.push_required();
		    for(uint s_row = 0; s_row < 9; ++s_row)
			st.add_required_entry(81*s_row + 9*s_col + s_val);
		}
		for(uint s_box_r = 0; s_box_r < 3; ++s_box_r){
		    for(uint s_box_c = 0; s_box_c < 3; ++s_box_c){
			st.push_required();
			for(uint s_idx_r = 0; s_idx_r < 3; ++s_idx_r)
			    for(uint s_idx_c = 0; s_idx_c < 3; ++s_idx_c)
				st.add_required_entry(81*(3*s_box_r+s_idx_r) +
					9*(3*s_box_c+s_idx_c) + s_val);
		    }
		}
	    }
	    for(uint s_unit = 0; s_unit < 81; ++s_unit){
		st.push_required();
		for(uint s_val = 0; s_val < 9; ++s_val)
		    st.add_required_entry(9*s_unit + s_val);
	    }
	}
	std::array<char const *const, 50> input96 = {{
#include "sudoku.include"
	}};
    }
    uint problem96(){
	using namespace euler96;
	algx_state st(81*9);
	sudoku_base(st);
	uint ct = 0;
	uint sum = 0;
	for(char const *c : input96){
	    st.clean();
	    while(ct > 0){
		st.pop_required();
		--ct;
	    }
	    for(uint s_unit = 0; s_unit < 81; ++s_unit){
		if(*c != '0'){
		    ++ct;
		    st.push_required();
		    st.add_required_entry(9*s_unit + *c-'1');
		}
		++c;
	    }
	    assert(*c == '\0');
	    if(!st.next_solution()) {
		return 0;
	    } else {
		std::vector<uint> s = st.read_solution();
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
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set9
    {{P(90),P(91),P(92),P(93),P(94),P(95),P(96),P(97)}};
}
