#include "set9.hh"
#include "util.hh"

#include <bitset>
#include <forward_list>

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
		    for(digit_iterator di(n); di != digit_iterator(); ++di)
			next += *di * *di;
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
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set9
    {{P(90),P(91),P(92)}};
}
