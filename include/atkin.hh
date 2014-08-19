// Copyright 2014 David Adler

#ifndef _ATKIN_HH_
#define _ATKIN_HH_

#include "util.hh"

#include <cassert>
#include <vector>

namespace euler {
class prime_iterator {
    class delta_iterator {
	static u8 const table[60];
	u8 delta;
    public:
	static u8 const bad_delta = 255;
	static u8 const delta_index_table[];
	explicit delta_iterator(u8 const d = 2) : delta(d) {}
	u64 operator*() const {
	    return delta;
	}
	delta_iterator& operator++(){
	    delta = table[delta];
	    assert(delta != bad_delta);
	    return *this;
	}
	delta_iterator operator++(int){
	    u8 const prev = delta;
	    operator++();
	    return delta_iterator(prev);
	}
    };
    typedef std::vector<bool> b_vec;
    static b_vec pre_primes;
    static std::vector<b_vec> primes;
    static u64 constexpr ks_per_cycle = 32;
    static b_vec extend_primes();
    static b_vec::const_iterator get_vec_iter(u64 x);
    delta_iterator delta_iter;
    u64 k;
    b_vec::const_iterator vec_iter;
    void advance();
    prime_iterator(u8 const d, u64 k0, b_vec::const_iterator const& vi)
	: delta_iter(d), k(k0), vec_iter(vi) {}
    prime_iterator(u8 const d, u64 k0, b_vec::const_iterator && vi)
	: delta_iter(d), k(k0), vec_iter(vi) {}
public:
    prime_iterator() : delta_iter(), k(0), vec_iter(pre_primes.cbegin()) {}
    prime_iterator(u64 lower) {
	assign(lower);
    }
    void assign(u64 lower);
    u64 operator*() const {
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
