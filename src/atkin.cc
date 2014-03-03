#include "atkin.hh"

#include <cassert>
#include <vector>
#include <array>
#include <algorithm>

#ifndef NO_THREADS
#include <mutex>
#include <condition_variable>
#endif

namespace euler {
    struct fg_pair {
	u8 f;
	u8 g;
    };
    template <i32 pair_ct>
	struct fg_info {
	    u8 d;
	    std::array<fg_pair, pair_ct> pairs;
	};
    std::array<fg_info<16>, 8> constexpr a_info
    {{{1, {{{2, 15}, {3, 5}, {3, 25}, {5, 9}, {5, 21}, {7, 15},
		  {8, 15}, {10, 9}, {10, 21}, {12, 5}, {12, 25}, {13, 15},
		  {15, 1}, {15, 11}, {15, 19}, {15, 29}}}},
	{13, {{{1, 3}, {1, 27}, {4, 3}, {4, 27}, {6, 7}, {6, 13}, {6, 17},
		     {6, 23}, {9, 7}, {9, 13}, {9, 17}, {9, 23}, {11, 3},
		     {11, 27}, {14, 3}, {14, 27}}}},
	{17, {{{2, 1}, {2, 11}, {2, 19}, {2, 29}, {7, 1}, {7, 11}, {7, 19},
		     {7, 29}, {8, 1}, {8, 11}, {8, 19}, {8, 29}, {13, 1},
		     {13, 11}, {13, 19}, {13, 29}}}},
	{29, {{{1, 5}, {1, 25}, {4, 5}, {4, 25}, {5, 7}, {5, 13}, {5, 17},
		     {5, 23}, {10, 7}, {10, 13}, {10, 17}, {10, 23}, {11, 5},
		     {11, 25}, {14, 5}, {14, 25}}}},
	{37, {{{2, 9}, {2, 21}, {3, 1}, {3, 11}, {3, 19}, {3, 29}, {7, 9},
		     {7, 21}, {8, 9}, {8, 21}, {12, 1}, {12, 11}, {12, 19},
		     {12, 29}, {13, 9}, {13, 21}}}},
	{41, {{{2, 5}, {2, 25}, {5, 1}, {5, 11}, {5, 19}, {5, 29}, {7, 5},
		      {7, 25}, {8, 5}, {8, 25}, {10, 1}, {10, 11}, {10, 19},
		      {10, 29}, {13, 5}, {13, 25}}}},
	{49, {{{1, 15}, {4, 15}, {5, 3}, {5, 27}, {6, 5}, {6, 25}, {9, 5},
		      {9, 25}, {10, 3}, {10, 27}, {11, 15}, {14, 15}, {15, 7},
		      {15, 13}, {15, 17}, {15, 23}}}},
	{53, {{{1, 7}, {1, 13}, {1, 17}, {1, 23}, {4, 7}, {4, 13}, {4, 17},
		      {4, 23}, {11, 7}, {11, 13}, {11, 17}, {11, 23}, {14, 7},
		      {14, 13}, {14, 17}, {14, 23}}}}}};
    std::array<fg_info<12>, 4> constexpr b_info
    {{{7, {{{1, 2}, {1, 8}, {1, 22}, {1, 28}, {3, 10}, {3, 20}, {7, 10},
		  {7, 20}, {9, 2}, {9, 8}, {9, 22}, {9, 28}}}},
	{19, {{{1, 4}, {1, 14}, {1, 16}, {1, 26}, {5, 2}, {5, 8}, {5, 22},
		     {5, 28}, {9, 4}, {9, 14}, {9, 16}, {9, 26}}}},
	{31, {{{3, 2}, {3, 8}, {3, 22}, {3, 28}, {5, 4}, {5, 14}, {5, 16},
		     {5, 26}, {7, 2}, {7, 8}, {7, 22}, {7, 28}}}},
	{43, {{{1, 10}, {1, 20}, {3, 4}, {3, 14}, {3, 16}, {3, 26}, {7, 4},
		      {7, 14}, {7, 16}, {7, 26}, {9, 10}, {9, 20}}}}}};
    std::array<fg_info<24>, 4> constexpr c_info
    {{{11, {{{2, 1}, {2, 11}, {2, 19}, {2, 29}, {3, 4}, {3, 14}, {3, 16},
		   {3, 26}, {5, 2}, {5, 8}, {5, 22}, {5, 28}, {7, 4}, {7, 14},
		   {7, 16}, {7, 26}, {8, 1}, {8, 11}, {8, 19}, {8, 29},
		   {10, 7}, {10, 13}, {10, 17}, {10, 23}}}},
	{23, {{{1, 10}, {1, 20}, {2, 7}, {2, 13}, {2, 17}, {2, 23}, {3, 2},
		     {3, 8}, {3, 22}, {3, 28}, {4, 5}, {4, 25}, {6, 5}, {6, 25},
		     {7, 2}, {7, 8}, {7, 22}, {7, 28}, {8, 7}, {8, 13}, {8, 17},
		     {8, 23}, {9, 10}, {9, 20}}}},
	{47, {{{1, 4}, {1, 14}, {1, 16}, {1, 26}, {2, 5}, {2, 25}, {3, 10},
		      {3, 20}, {4, 1}, {4, 11}, {4, 19}, {4, 29}, {6, 1},
		      {6, 11}, {6, 19}, {6, 29}, {7, 10}, {7, 20}, {8, 5},
		      {8, 25}, {9, 4}, {9, 14}, {9, 16}, {9, 26}}}},
	{59, {{{1, 2}, {1, 8}, {1, 22}, {1, 28}, {4, 7}, {4, 13}, {4, 17},
		      {4, 23}, {5, 4}, {5, 14}, {5, 16}, {5, 26}, {6, 7},
		      {6, 13}, {6, 17}, {6, 23}, {9, 2}, {9, 8}, {9, 22},
		      {9, 28}, {10, 1}, {10, 11}, {10, 19}, {10, 29}}}}}};
#define B bad_delta
    u8 const prime_iterator::delta_iterator::table[] =
//	  0  1  2  3  4  5  6  7
	{ B, 7, 3, 5, B, 1, B,11,
	  B, B, B,13, B,17, B, B,
	  B,19, B,23, B, B, B,29,
	  B, B, B, B, B,31, B,37,
	  B, B, B, B, B,41, B, B,
	  B,43, B,47, B, B, B,49,
	  B,53, B, B, B,59, B, B,
	  B, B, B, 1};
    u8 const prime_iterator::delta_iterator::delta_index_table[] =
//	  0  1  2  3  4  5  6  7
	{ B, 0, B, B, B, B, B, 1,
	  B, B, B, 2, B, 3, B, B,
	  B, 4, B, 5, B, B, B, 6,
	  B, B, B, B, B, 7, B, 8,
	  B, B, B, B, B, 9, B, B,
	  B,10, B,11, B, B, B,12,
	  B,13, B, B, B,14, B, B,
	  B, B, B,15};
#undef B
#define D_X_TAB delta_iterator::delta_index_table
    auto prime_iterator::extend_primes() -> b_vec {
	b_vec vec(ks_per_cycle * 16, false);
	u64 const start = ks_per_cycle * primes.size();
	for(auto const& info : a_info)
	    for(auto const& pair : info.pairs){
		long x = pair.f;
		long y0 = pair.g;
		long k0 = (4*x*x + y0*y0 - info.d) / 60;
		assert((4*x*x + y0*y0 - info.d) % 60 == 0);
		while(k0 < (long) (start + ks_per_cycle)){
		    k0 += x + x + 15;
		    x += 15;
		}
		while(true){
		    x -= 15;
		    k0 -= x + x + 15;
		    if(x < 0)
			break;
		    while(k0 < (long) (start)){
			k0 += y0 + 15;
			y0 += 30;
		    }
		    long k = k0;
		    long y = y0;
		    while(k < (long) (start + ks_per_cycle)){
			u64 const index = 16*(k-start)+D_X_TAB[info.d];
			vec[index] = !vec[index];
			k += y + 15;
			y += 30;
		    }
		}
	    }
	for(auto const& info : b_info)
	    for(auto const& pair : info.pairs){
		long x = pair.f;
		long y0 = pair.g;
		long k0 = (3*x*x + y0*y0 - info.d) / 60;
		assert((3*x*x + y0*y0 - info.d) % 60 == 0);
		while(k0 < (long) (start + ks_per_cycle)){
		    k0 += x + 5;
		    x += 10;
		}
		while(true){
		    x -= 10;
		    k0 -= x + 5;
		    if(x < 0)
			break;
		    while(k0 < (long) (start)){
			k0 += y0 + 15;
			y0 += 30;
		    }
		    long k = k0;
		    long y = y0;
		    while(k < (long) (start + ks_per_cycle)){
			u64 const index = 16*(k-start)+D_X_TAB[info.d];
			vec[index] = !vec[index];
			k += y + 15;
			y += 30;
		    }
		}
	    }
	for(auto const& info : c_info)
	    for(auto const& pair : info.pairs){
		long x = pair.f;
		long y0 = pair.g;
		long k0 = (3*x*x - y0*y0 - info.d) / 60;
		assert((3*x*x - y0*y0 - (long)info.d) % 60 == 0);
                while(true){
		    while(k0 >= (long)(start + ks_per_cycle)){
			if(x <= y0)
			    goto break_scan;
			k0 -= y0 + 15;
			y0 += 30;
		    }
		    long k = k0;
		    long y = y0;
		    while(k >= (long) start && y < x){
			u64 const index = 16*(k-start)+D_X_TAB[info.d];
			vec[index] = !vec[index];
			k -= y + 15;
			y += 30;
		    }
		    k0 += x + 5;
		    x += 10;
		}
                break_scan:;
	    }
	u64 const bottom = start * 60;
	u64 const top = (start + ks_per_cycle) * 60;
	u64 k = 0;
	delta_iterator delta(1);
	auto outer = primes.cbegin();
	if(start == 0)
	    vec[0] = false;
	while(true){
	    auto const& old_vec = outer == primes.cend() ? vec : *outer++;
	    auto iter = old_vec.cbegin();
	    assert(k % ks_per_cycle == 0 && *delta == 1);
	    while(iter != old_vec.cend()){
		if(*iter){
		    u64 p = k * 60 + *delta;
		    u64 const p_sq = p * p;
		    if(p_sq >= top)
			goto done;
		    p = (bottom/p_sq + 1)*p_sq;
		    while(p < top){
			u64 const d_x = D_X_TAB[p % 60];
			if(d_x != delta_iterator::bad_delta){
			    u64 const k_x = p / 60 - start;
			    assert(p / 60 >= start && k_x < ks_per_cycle);
			    vec[k_x * 16 + d_x] = false;
			}
			p += p_sq;
		    }
		}
		++iter;
		if(*++delta == 1)
		    ++k;
	    }
	    assert(&old_vec != &vec); //should have hit p_sq>=top
	}
done:
	return vec;
    }
    prime_iterator::b_vec prime_iterator::pre_primes = { true, true, true };
    std::vector<prime_iterator::b_vec> prime_iterator::primes;
    void prime_iterator::advance() {
	do {
	    ++vec_iter;
	    if(*++delta_iter == 1){
		if(k == 0){ //handle special case for k==0 meaning two different things
		    if(vec_iter == pre_primes.cend())
			vec_iter = get_vec_iter(0);
		    else
			k = 1;
		} else if(++k % ks_per_cycle == 0){
		    vec_iter = get_vec_iter(k / ks_per_cycle);
		}
	    }
	} while(!*vec_iter);
    }
    auto prime_iterator::get_vec_iter(u64 x) -> b_vec::const_iterator {
#ifndef NO_THREADS
	typedef std::unique_lock<std::mutex> lock_t;
	typedef std::condition_variable excon_t;
	static std::mutex read_mutex;
	static excon_t *extension_condition;
	lock_t lock(read_mutex);
	while(x >= primes.size()){
	    if(extension_condition == nullptr){
		extension_condition = new excon_t;
		lock.unlock();
		/* extend_primes can safely read primes (but not modify it) because
		 * only reads will occur so long as extension_condition != nullptr
		 * (except by this thread, below).
		 * I.e., so long as the lock is released by this thread,
		 * a) no other thread will write to primes, and
		 * b) this thread will not write to primes
		 */
		b_vec vec = extend_primes();
		if(primes.size() == 0)
		    vec[0] = false;
		lock.lock();
		primes.push_back(std::move(vec));
		excon_t *excon = extension_condition;
		extension_condition = nullptr;
		excon->notify_all();
		delete excon;
		// the standard specifically allows deletion of excon even
		// though other threads will not return from wait(lock) until
		// we return from get_vec_iter below, but there used to be a
		// bug in libstdc++ which broke this...
	    } else {
		extension_condition->wait(lock);
	    }
	    assert(lock.owns_lock());
	}
#else
	while(x >= primes.size()){
	    primes.push_back(extend_primes());
	    if(primes.size() == 1)
		primes.front()[0] = false;
	}
#endif
	assert(primes[x].size() == ks_per_cycle * 16);
	return primes[x].cbegin();
    }
    void prime_iterator::assign(u64 lower){
	if(lower <= 5){
	    *this = prime_iterator();
	    if(lower > 2){
		++vec_iter;
		++delta_iter;
		if(lower > 3){
		    ++vec_iter;
		    ++delta_iter;
		}
	    }
	} else {
	    u64 l_d = lower % 60;
	    while(delta_iterator::delta_index_table[l_d] ==
		    delta_iterator::bad_delta)
		++l_d;
	    delta_iter = delta_iterator(l_d);
	    k = lower / 60;
	    vec_iter = get_vec_iter(k/ks_per_cycle) + (k % ks_per_cycle) * 16
		+ delta_iterator::delta_index_table[*delta_iter];
	    if(!*vec_iter)
		advance();
	}
    }
}
