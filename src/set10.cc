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
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set10
    {{P(100),P(101)}};
}
