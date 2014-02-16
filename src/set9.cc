#include "set9.hh"
#include "util.hh"

#include <bitset>

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
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set9
    {{P(90),P(91)}};
}
