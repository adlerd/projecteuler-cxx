// Copyright 2014 David Adler

#include "util.hh"
#include "atkin.hh"

#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <queue>

namespace {
    using namespace euler;
    namespace euler120 {
	u32 r_max(u16 const a){
	    u32 mod = a*a;
	    u32 const x = a-1;
	    u32 const y = a+1;
	    u32 xx = x;
	    u32 yy = y;
	    u32 max_r = 2;
	    do {
		u32 r = (xx+yy) % mod;
		if(r > max_r)
		    max_r = r;
		xx = (xx*x) % mod;
		yy = (yy*y) % mod;
	    } while(x != xx || y != yy);
	    return max_r;
	}
    }
    u32 problem120(){ 
	using namespace euler120;
	u32 ret = 0;
	for(u16 a = 3; a < 1001; ++a)
	    ret += r_max(a);
	return ret;
    }
    bigint problem121(){
	typedef mpq_class bigq;
	bigq total(0);
	bigq chance(1);
	u8 constexpr limit = 15;
	for(u8 ct = 1+limit/2; ct <= limit; ++ct){
	    u64 const start = (1 << ct) - 1;
	    u64 pat = start;
	    do {
		chance = 1;
		for(u8 round = 0; round < limit; ++round){
		    if((pat & (1 << round)) == 0)
			chance *= 1 + round;
		    chance /= 2 + round;
		}
		total += chance;
	    } while((pat = gosper(limit, ct, pat)) != start);
	}
	return total.get_den() / total.get_num();
    }
    u32 problem122(){
	u8 constexpr MAX = 200;
	std::vector<u8> tree(1,1);
	tree.reserve(1 << 26);
	std::vector<u8> found(MAX,255);
	found[0] = 0;
	u8 maxfound = 1;
	std::vector<u32> parents(1,0);// index 0 is bottom parent
	std::vector<u32> children_remaining(1,1);
	while(maxfound < MAX){
	    while(true){
		for(u8 sum_idx = 0; sum_idx < parents.size(); ++sum_idx){
		    u16 sum = ((u16)tree[parents[0]]) + ((u16)tree[parents[sum_idx]]);
		    sum = std::min(sum, (u16) 255);
		    tree.push_back((u8) sum);
		    if(sum <= MAX && found[sum-1] == 255){
			found[sum-1] = parents.size();
			while(maxfound < MAX && found[maxfound] != 255)
			    ++maxfound;
		    }
		}
		children_remaining[0] = 1;
		u8 plevel = 0;
		while(--children_remaining[plevel] == 0){
		    children_remaining[plevel] = parents.size() - plevel;
		    ++parents[plevel];
		    if(++plevel == parents.size())
			goto next_level;
		}
	    }
next_level:
	    parents.push_back(0);
	    for(u8 i = 0; i < children_remaining.size(); ++i)
		++children_remaining[i];
	    children_remaining.push_back(1);
	}
	u32 sum = 0;
	for(u8 i = 0; i < MAX; ++i)
	    sum += found[i];
	return sum;
    }
    u32 problem123(){
	u64 constexpr target = 10000000000;
	prime_iterator pi;
	u32 idx = 1;
	while(true){
	    u64 p = *pi;
	    bigint a, b;
	    mpz_ui_pow_ui(a.get_mpz_t(), p - 1, idx);
	    mpz_ui_pow_ui(b.get_mpz_t(), p + 1, idx);
	    if((a + b) % (p * p) > target)
		return idx;
	    ++idx;
	    ++pi;
	}
    }
    u32 problem124(){
	std::vector<std::pair<u32, u32>> vec;
	u32 constexpr limit = 100000;
	u32 constexpr elt = 10000;
	vec.reserve(limit);
	for(u32 n = 1; n <= limit; ++n){
	    u32 rad = 1;
	    for(auto& p : ct_factors(n))
		rad *= p.first;
	    vec.push_back(std::make_pair(rad, n));
	}
	auto item = vec.begin() + (elt - 1);
	std::nth_element(vec.begin(), item, vec.end());
	return item->second;
    }
    u64 problem125(){
	u64 constexpr target = 100000000;
	u64 left = 1;
	std::vector<u64> values;
	while(left * left < target){
	    u64 right = left + 1;
	    u64 current = (left * left) + (right * right);
	    while(current < target){
		if(is_palindrome(std::to_string(current)))
		    values.push_back(current);
		++right;
		current += right * right;
	    }
	    ++left;
	}
	left = 0;
	u64 sum = 0;
	std::sort(values.begin(), values.end());
	for(u64 v : values){
	    if(v != left){
		left = v;
		sum += v;
	    }
	}
	return sum;
    }
    namespace euler126 {
	class layer_gen;
	typedef std::unique_ptr<layer_gen> lgp;
	class layer_gen {
	public:
	    u32 value;
	    explicit layer_gen(u32 v) : value(v) {}
	    virtual ~layer_gen(){}
	    virtual void generate_children(std::vector<lgp>& into) = 0;
	};
	class more_layers : public layer_gen {
	    u32 const b;
	    u32 const c;
	    u32 n;
	public:
	    more_layers(u32 bb, u32 cc, u32 nn) : b(bb), c(cc), n(nn),
	            layer_gen(4*nn*nn+bb*nn+cc){}
	    virtual ~more_layers(){}
	    virtual void generate_children(std::vector<lgp>& into){
		++n;
		value = 4*n*n+b*n+c;
	    }
	};
	class first_layer : public layer_gen {
	    u32 x;
	    u32 const y;
	    u32 const z;
	public:
	    first_layer(u32 xx, u32 yy, u32 zz) : x(xx), y(yy), z(zz),
	            layer_gen(2*(xx*yy + xx*zz + yy*zz)) {
		assert(x >= y && y >= z);
	    }
	    virtual ~first_layer(){}
	    virtual void generate_children(std::vector<lgp>& into){
		u32 const b = (x+y+z-3)*4;
		into.emplace_back(new more_layers(b, value - 4 - b, 2));
		if(x == y+1)
		    into.emplace_back(new first_layer(x,x,z));
		else if(x == y && x == z+1)
		    into.emplace_back(new first_layer(x,x,x));
		++x;
		value = 2*(x*y + x*z + y*z);
	    }
	};
	bool compare_layer_up(lgp& a, lgp& b){
	    return a->value > b->value;
	}
    }
    u32 problem126(){
	using namespace euler126;
	std::vector<lgp> heap;
	heap.emplace_back(new first_layer(1,1,1));
	u32 prev = 0;
	u32 ct = 0;
	while(true){
	    std::pop_heap(heap.begin(), heap.end(), &compare_layer_up);
	    layer_gen *p = heap.back().get();
	    u32 current = p->value;
	    if(current == prev) {
		++ct;
	    } else if(ct == 1000) {
		return prev;
	    } else {
		prev = current;
		ct = 1;
	    }
	    size_t s = heap.size();
	    p->generate_children(heap);
	    while(s <= heap.size()){
		std::push_heap(heap.begin(), heap.begin() + s,
			&compare_layer_up);
		++s;
	    }
	}
    }
    u64 problem127(){
	u32 constexpr limit = 120000;
	// initialize the cache of radicals
	std::vector<u32> rad_cache(limit-1, 0);
	for(u32 a = 1; a < limit; ++a){
	    u32 r = 1;
	    auto const cfs = ct_factors(a);
	    for(auto& p : cfs)
		r *= p.first;
	    rad_cache.at(a-1) = r;
	}
	// initialize the coprime-pair generator of pairs (b,a).
	std::vector<std::pair<u32, u32>> relprim_gen;
	relprim_gen.emplace_back(2,1);
	relprim_gen.emplace_back(3,1);
	// done
	u64 sum = 0;
	u32 ct = 0;
	while(!relprim_gen.empty()){
	    auto p = relprim_gen.back();
	    relprim_gen.pop_back();
	    u32 const b = p.first;
	    u32 const a = p.second;
	    // if p | a and ~(p | b), then ~(p | a+b),
	    // so in fact you only have to "check" relative primality between a
	    // and b which is already done, so no gcd checking required at all!
	    u32 const c = a + b;
	    if(c < limit){
		if(((u64)rad_cache[b-1]) * ((u64)rad_cache[a-1]) *
			((u64)rad_cache[c-1]) < c){
		    ++ct;
		    sum += c;
		}
		// now reinsert children
		// the generator is nondecreasing in both a and b
		if(2*b-a < limit)
		    relprim_gen.emplace_back(2*b-a, b);
		if(2*b+a < limit)
		    relprim_gen.emplace_back(2*b+a, b);
		if(b+2*a < limit)
		    relprim_gen.emplace_back(b+2*a, a);
	    }
	}
	return sum;
    }
    namespace euler128 {
	typedef std::pair<u64, std::shared_ptr<bool>> entry;
	typedef std::priority_queue<entry, std::vector<entry>, std::greater<entry>> pq;
    }
    u64 problem128() {
	using namespace euler128;
	u32 const target = 2000;
	// the general idea here is three cooperative coroutines, of which two
	// are represented by priority queues and the last by a primes
	// iterator.
	// the first coroutine is ordered by the base value
	// which is either 3n^2+3n+2 or 3n^2+9n+7 for n>0.
	// the coroutine advances when the head's ptr is unique
	u32 passed_entries = 1; // skip "1"
	pq sequence;
	// the second coroutine is ordered by the difference to be checked
	// which is 6n+7, 12n+17, 6n+11, or 12n+5
	// the coroutine advances when the prime exceeds its head
	pq to_check;
	// the final coroutine is checking if each prime is of the form 6n+5
	// in which case entries are added to the queues
	prime_iterator pi;
	while(true){
	    u64 const prime = *pi++;
	    // second coroutine
	    while(!to_check.empty() && prime > to_check.top().first){
		*to_check.top().second = false;
		to_check.pop();
	    }
	    while(!to_check.empty() && prime == to_check.top().first)
		to_check.pop();
	    // first coroutine
	    while(!sequence.empty()){
		if(*sequence.top().second){
		    if(sequence.top().second.unique()){
			if(++passed_entries == target)
			    return sequence.top().first;
		    } else {
			break;
		    }
		}
		// i.e., get here if
		// !*sequence.top().second ||
		//     (sequence.top().second.unique() &&
		//         passed_entries < target)
		sequence.pop();
	    }
	    // third coroutine
	    if(prime % 6 == 5){
		u64 const n = prime / 6;
		std::shared_ptr<bool> alpha(new bool(true));
		std::shared_ptr<bool> beta(new bool(true));
		to_check.emplace(prime+2, alpha);
		to_check.emplace(12*n+17, alpha);
		to_check.emplace(prime+6, beta);
		to_check.emplace(12*n+5, beta);
		sequence.emplace(3*n*(n+1)+2, std::move(alpha));
		sequence.emplace(3*n*(n+3)+7, std::move(beta));
	    }
	}
    }
    u64 problem129(){
	using namespace euler129;
	u64 constexpr target = 1000000;
	std::unordered_map<u32,u32> pmap;
	for(u64 tens = (target-target/40)/10*10; true; tens += 10)
	    for(u8 ones : {1, 3, 7, 9}){
		u32 lr = factor_lowrepunit(tens + ones, pmap);
		if(lr > target)
		    return tens + ones;
	    }
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set12
    {P(120),P(121),P(122),P(123),P(124),P(125),P(126),P(127),P(128),P(129)};
}
