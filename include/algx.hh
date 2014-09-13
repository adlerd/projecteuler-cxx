// Copyright 2014 David Adler

#ifndef _ALGX_HH_
#define _ALGX_HH_

#include <deque>
#include <forward_list>
#include "util.hh"

namespace euler {
class algx_state {
    friend struct algx_impl;
    struct element {
	element *up;
	element *down;
	element *left;
	element *right;
	element *header;
	// size and optional only set in header; size does not include header
	// size is actually row id in row headers
	u32 size : 31;
	bool optional : 1;
    };
    typedef element *elt;
    std::deque<element> store;
    elt const origin;
    std::forward_list<elt> state;

    void clean_one();
    bool unclean_one();
    bool search_down();
    bool search_up();

public:
    /* constructors */
    explicit algx_state(u32 rowcount);
    algx_state(algx_state const&) = delete;
    algx_state(algx_state&&) = delete;
    void operator=(algx_state const&) = delete;
    void operator=(algx_state&&) = delete;

    /* query whether state is "clean"; i.e., whether we are iterating through
     * solutions (dirty) or are able to modify the constraint stacks (clean) */
    bool is_clean() const {
	return state.empty();
    }

    /* manipulate the (conceptual) required and optional constraint stacks */
    void push_required();
    void pop_required();
    void push_optional();
    void pop_optional();
    /* modify the top element of one of the constraint stacks by adding a key
     * which will satisfy it */
    void add_required_entry(u32 key);
    void add_optional_entry(u32 key);

    /* next_solution can be called when dirty or clean, and dirty status is
     * indicated by return: iff there are no further solutions, next_solution()
     * returns false and subsequent calls to is_clean() will return true;
     * otherwise next_solution() returns true, state is dirty, and a solution
     * may be read by read_solution() */
    bool next_solution(){
	if(is_clean())
	    return search_down();
	else
	    return search_up();
    }

    /* read_solution must be called when dirty, and returns the current set
     * of keys which satisfy the constraints */
    std::vector<u32> read_solution() const;
    void clean() {
	while(!is_clean()){
	    clean_one();
	    state.pop_front();
	}
    }
};
}

#endif
