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
	uint size : 31;
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
    explicit algx_state(uint rowcount);
    algx_state(algx_state const&) = delete;
    algx_state(algx_state&&) = delete;
    void operator=(algx_state const&) = delete;
    void operator=(algx_state&&) = delete;

    /* query */
    bool is_clean() const {
	return state.empty();
    }

    /* functions modifying "clean" state */
    void push_required();
    void pop_required();
    void push_optional();
    void pop_optional();
    void add_required_entry(uint key);
    void add_optional_entry(uint key);

    /* next_solution can be called when dirty or clean, and dirty status is
     * indicated by return */
    bool next_solution(){
	if(is_clean())
	    return search_down();
	else
	    return search_up();
    }
    /* read_solution must be called when dirty */
    std::vector<uint> read_solution() const;
    void clean() {
	while(!is_clean()){
	    clean_one();
	    state.pop_front();
	}
    }
};
}

#endif
