#include "util.hh"

#include <vector>

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
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set10
    {{P(100),P(101),P(102)}};
}
