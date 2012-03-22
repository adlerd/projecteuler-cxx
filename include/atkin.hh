#ifndef _ATKIN_HH_
#define _ATKIN_HH_

#include <cassert>
#include <vector>

namespace euler {
    typedef unsigned long ulong;
class prime_iterator {
    class delta_iterator {
	static unsigned char const table[60];
	unsigned char delta;
    public:
	static unsigned char const bad_delta = 255;
	static unsigned char const delta_index_table[];
	explicit delta_iterator(unsigned char const d = 2) : delta(d) {}
	ulong operator*() const {
	    return delta;
	}
	delta_iterator& operator++(){
	    delta = table[delta];
	    assert(delta != bad_delta);
	    return *this;
	}
	delta_iterator operator++(int){
	    unsigned char const prev = delta;
	    operator++();
	    return delta_iterator(prev);
	}
    };
    typedef std::vector<bool> b_vec;
    static b_vec pre_primes;
    static std::vector<b_vec> primes;
    static ulong constexpr ks_per_cycle = 32;
    static b_vec extend_primes();
    static b_vec::const_iterator get_vec_iter(ulong x);
    delta_iterator delta_iter;
    ulong k;
    b_vec::const_iterator vec_iter;
    void advance();
    prime_iterator(unsigned char const d, ulong k0, b_vec::const_iterator const& vi)
	: delta_iter(d), k(k0), vec_iter(vi) {}
    prime_iterator(unsigned char const d, ulong k0, b_vec::const_iterator && vi)
	: delta_iter(d), k(k0), vec_iter(vi) {}
public:
    prime_iterator() : delta_iter(), k(0), vec_iter(pre_primes.cbegin()) {}
    ulong operator*() const {
	assert(*vec_iter);
	return k * 60 + *delta_iter;
    }
    prime_iterator& operator++(){
	assert(*vec_iter);
	advance();
	return *this;
    }
    prime_iterator operator++(int){
	assert(*vec_iter);
	prime_iterator tmp(*this);
	advance();
	return tmp;
    }
};
}

#endif
