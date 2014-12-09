// Copyright 2014 David Adler

#include "util.hh"
#include "atkin.hh"
#include "algx.hh"

#include <forward_list>
#include <queue>

namespace {
    using namespace euler;
    bigint problem110(){
	using namespace euler108;
	u64 constexpr minlim = 4000000;
	u64 constexpr search = 20000;
	bigint min = smallestN(minlim);
	bigint tmp;
	for(u64 t = minlim + 1; t < minlim + search; ++t){
	    tmp = smallestN(t);
	    if(tmp < min)
		std::swap(tmp, min);
	}
	return min;
    }
    namespace euler111 {
	bool incr_digits(std::vector<u8>& digs, u8 skip, u8 low_nonzero){
	    for(u8 i = 0; i < digs.size(); ++i){
		if(digs[i] < (skip == 9 ? 8 : 9)){
		    if(++digs[i] == skip)
			++digs[i];
		    return true;
		} else {
		    digs[i] = (skip == 0 || (i == 0 && low_nonzero)) ? 1 : 0;
		    // and go on to increment the next digit
		}
	    }
	    return false;
	}
	struct result {
	    u64 sum;
	    u32 instances;
	    u8 maxct;
	};
	result maxrep(u8 const n, u8 const d){
	    result ret{0,0,n};
	    while(ret.instances == 0){
		--ret.maxct;
		assert(ret.maxct > 0);
		// find all of the positions for the digit
		u8 const k = ret.maxct;
		u64 const start = (1 << k) - 1;
		u64 locs = start;
		bool const locs_low_zero = d == 0;
		/* we will cycle through locs with gosper(), but...
		 * the lowest bit of locs controls the highest
		 * digit, so if d==0 then treat locs as an (n-1)-bit
		 * value and always use it with its
		 * low bit unset indicating to use other_digits */
		do { // for each k-subset of digits (in locs)
		    std::vector<u8> other_digs(n - k, d == 0 ? 1 : 0);
		    bool const od_low_nonzero = locs_low_zero || (locs % 2 == 0);
		    if(od_low_nonzero)
			other_digs[0] = 1;
		    /* that is, make sure that the "low" digit in other_digs
		     * is nonzero, iff it becomes the high digit */
		    do { // for each value of other_digs
			// reconstruct the number
			u64 val = 0;
			u64 locs_mut = locs_low_zero ? locs << 1 : locs;
			u8 od_x = 0;
			while(od_x < other_digs.size() || locs_mut != 0){
			    val *= 10;
			    if((locs_mut & 1) == 0){
				assert(od_x < other_digs.size());
				val += other_digs[od_x++];
			    } else {
				val += d;
			    }
			    locs_mut >>= 1;
			}
			if(is_prime(val)){
			    ++ret.instances;
			    ret.sum += val;
			}
		    } while(incr_digits(other_digs, d, od_low_nonzero));
		} while((locs = gosper(locs_low_zero ? n-1 : n, k, locs)) != start);
	    }
	    return ret;
	}
    }
    u64 problem111(){
	using namespace euler111;
	u64 ret = 0;
	for(int i = 0; i < 10; ++i)
	    ret += maxrep(10, i).sum;
	return ret;
    }
    u64 problem112(){
	u32 n = 0;
	u32 d = 100;
	while(d * 99 != n * 100){
	    ++d;
	    int prev_cmp = 0;
	    digit_iterator di(d);
	    u8 prev = *di++;
	    while(di != digit_iterator()){
		u8 current = *di++;
		if(prev_cmp == 0){
		    prev_cmp = ((int)current) - prev;
		} else {
		    if(current < prev){
			if(prev_cmp > 0){
			    ++n;
			    break;
			}
		    } else if(current > prev){
			if(prev_cmp < 0){
			    ++n;
			    break;
			}
		    }
		}
		prev = current;
	    }
	}
	return d;
    }
    namespace euler113 {
	typedef std::vector<std::vector<u64>> mem_t;
	u64 increasing(mem_t& incr, u8 digct, u8 min){
	    if(min == 9 || digct == 0)
		return 1;
	    if(incr[digct][min] != 0)
		return incr[digct][min];
	    u64 ret = 0;
	    for(u8 i = min; i < 10; ++i)
		ret += increasing(incr, digct - 1, i);
	    incr[digct][min] = ret;
	    return ret;
	}
	u64 decreasing(mem_t& incr, u8 digct){
	    // numbers which, when reversed (and losing their trailing zeros),
	    // are increasing, but here we'll skip the all-same numbers
	    if(digct < 2)
		return 0;
	    u64 ret = decreasing(incr, digct-1);
	    for(u8 trailing = 0; trailing < digct; ++trailing)
		ret += increasing(incr, digct - trailing, 1);
	    return ret-9;
	}
    }
    u64 problem113(){
	using namespace euler113;
	u8 constexpr digs = 100;
	mem_t incr(digs+1,std::vector<u64>(10,0));
	return increasing(incr, digs, 0) + decreasing(incr, digs) - 1;
    }
    namespace euler114 {
	typedef std::vector<u64> mem_t;
	u64 start_red(mem_t& mem, u8 min_rlen, u32 len){
	    if(len < min_rlen)
		return 0;
	    if(mem[len] != 0)
		return mem[len];
	    u64 ret = 1; // the all-red solution
	    for(u32 rlen = min_rlen; rlen < len; ++rlen){
		ret += 1; // a red-then-all-black solution
		for(u32 blen = 1; blen < (len - rlen); ++blen)
		    ret += start_red(mem, min_rlen, len - rlen - blen);
	    }
	    mem[len] = ret;
	    return ret;
	}
	u64 fill_count(u8 min_rlen, u32 len){
	    mem_t mem(len+1);
	    u64 ret = 1; // the all-black solution
	    for(u8 blen = 0; blen < len; ++blen)
		ret += start_red(mem, min_rlen, len - blen);
	    return ret;
	}
    }
    u64 problem114(){
	using namespace euler114;
	return fill_count(3, 50);
    }
    u64 problem115(){
	using namespace euler114;
	u8 const min_rlen = 50;
	u32 len = min_rlen;
	while(fill_count(min_rlen, len) < 1000000)
	    ++len;
	return len;
    }
    namespace euler116 {
	typedef std::vector<u8> tileset;
	typedef std::vector<u64> mem_t;
	u64 with_tileset(mem_t& mem, tileset const& tiles, u64 len){
	    if(len == 0)
		return 1;
	    if(mem[len] != 0)
		return mem[len];
	    u64 ret = 0;
	    for(u8 tile : tiles)
		if(tile <= len)
		    ret += with_tileset(mem, tiles, len - tile);
	    mem[len] = ret;
	    return ret;
	}
    }
    u64 problem116(){
	using namespace euler116;
	u64 constexpr len = 50;
	u64 ret = 0;
	{
	    tileset tiles{1,2};
	    mem_t mem(len+1);
	    ret += with_tileset(mem, tiles, len) - 1;
	}
	{
	    tileset tiles{1,3};
	    mem_t mem(len+1);
	    ret += with_tileset(mem, tiles, len) - 1;
	}
	{
	    tileset tiles{1,4};
	    mem_t mem(len+1);
	    ret += with_tileset(mem, tiles, len) - 1;
	}
	return ret;
    }
    u64 problem117(){
	using namespace euler116;
	u64 constexpr len = 50;
	tileset tiles{1,2,3,4};
	mem_t mem(len+1);
	return with_tileset(mem, tiles, len);
    }
    namespace euler118 {
	u64 part_a(){
	    /* since our algx is built up by constraint first (i.e. each
	     * digit), and some primes are automatically disqualified by
	     * containing duplicates, first sort through all the primes on a
	     * different data structure */
	    std::vector<std::forward_list<u32>>
		    digstacks(9, std::forward_list<u32>({0}));
	    prime_iterator pi;
	    u32 ct = 1;
	    while(*pi < 10000000){
		u32 p = *pi++;
		digit_iterator di(p);
		bool killed = false;
		while(di != digit_iterator()){
		    int n = ((int)*di++) - 1;
		    if(n == -1 || digstacks[n].front() == ct){
			// kill this prime
			for(n = 0; n < 9; ++n)
			    if(digstacks[n].front() == ct)
				digstacks[n].pop_front();
			killed = true;
			break;
		    }
		    digstacks[n].push_front(ct);
		}
		if(!killed)
		    ++ct;
	    }
	    algx_state state(ct-1);
	    for(auto const& dig : digstacks){
		state.push_required();
		for(auto key : dig){
		    if(key == 0)
			break;
		    state.add_required_entry(ct-key-1);
		}
	    }
	    u64 solct = 0;
	    while(state.next_solution())
		++solct;
	    return solct;
	}
    }
    u64 problem118(){
	using namespace euler118;
	/* two parts to solution:
	 * run Algorithm X on primes with 7 or fewer digits;
	 * 8 and 9 will be special cases. */
	u64 ret = part_a();
	/* for the special cases:
	 * check whether each permutation of the 9 digits is prime
	 * or if the first digit is prime and the last 8 are prime */
	std::array<u8, 9> digs{1,2,3,4,5,6,7,8,9};
	do {
	    if(is_prime(from_digits(digs.cbegin(), digs.cend())))
		++ret;
	    if(is_prime(digs.front()) &&
		    is_prime(from_digits(digs.cbegin()+1, digs.cend())))
		++ret;
	} while(std::next_permutation(digs.begin(), digs.end()));
	return ret;
    }
    bigint problem119(){
	typedef std::pair<bigint, u64> pow_base;
	typedef std::priority_queue<pow_base, std::vector<pow_base>,
		std::greater<pow_base>> pq_t;
	pow_base next_new(9,3);
	pq_t pq;
	pq.push(std::make_pair(4, 2));
	pow_base top;
	u8 n = 0;
	while(true){
	    if(pq.top().first >= next_new.first){
		top = next_new;
		++next_new.second;
		next_new.first = next_new.second * next_new.second;
	    } else {
		top = pq.top();
		pq.pop();
	    }
	    if(digit_sum(top.first) == top.second)
		if(++n == 30)
		    return top.first;
	    top.first *= top.second;
	    pq.push(top);
	}
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set11
    {P(110),P(111),P(112),P(113),P(114),P(115),P(116),P(117),P(118),P(119)};
}
