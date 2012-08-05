#include "set6.hh"
#include "atkin.hh"
#include <map>
#include <list>

namespace euler {
    namespace euler60 {
	uint constexpr TARGET_DEPTH = 5;
	struct node;
	typedef std::map<ulong, bool> bmap;
	typedef std::list<node> nlist;
	struct node {
	    ulong head;
	    nlist children;
	    explicit node(ulong h) : head(h), children() {}
	};
	ulong next_10(ulong b){
	    ulong a = 1;
	    while(b > 0){
		a *= 10;
		b /= 10;
	    }
	    return a;
	}
	void make_matchmap(ulong prime, bmap& into){
	    into.clear();
	    ulong const factor_a = next_10(prime);
	    ulong factor_b = 10;
	    prime_iterator it(6);
	    ulong check_prime = 3;
	    while(check_prime < prime){
		while(factor_b <= check_prime)
		    factor_b *= 10;
		into[check_prime] = is_prime(factor_a * check_prime + prime)
		    && is_prime(factor_b * prime + check_prime);
		check_prime = *it++;
	    }
	}
	struct state {
	    ulong prime;
	    bmap match;
	    node top;
	    ulong minsum;
	    state() : prime(0), top(0), minsum(0) {}
	};
	bool refilter(node& from, ulong next_prime, uint depth, ulong sum){
	    nlist& ch = from.children;
	    if(sum < from.head)
		return true;
	    sum -= from.head;
	    uint const next_depth = depth + 1;
	    ch.remove_if([next_prime, next_depth, sum] (node& n) {
		    return refilter(n, next_prime, next_depth, sum); });
	    return ch.empty() && sum < next_prime * (TARGET_DEPTH - depth);
	}
	/* pre: prime should actually go in node */
	void insert_into_node(node& node, ulong cursum, uint depth,
		state& state){
	    cursum += node.head;
	    struct node next(state.prime);
	    if(depth == TARGET_DEPTH - 1){
		ulong prime = state.prime;
		assert (state.minsum == 0 || cursum < state.minsum);
		state.minsum = cursum + prime;
		node.children.push_back(next);
		refilter(state.top, state.prime, 0, state.minsum);
	    } else {
		for(struct node& n : node.children)
		    if(state.match[n.head])
			insert_into_node(n, cursum, depth + 1, state);
		node.children.push_back(next);
	    }
	}
	/* one might sacrifice algorithmic correctness for this faster solution:
	bool is_complete(state& state){
	    return state.minsum != 0;
	}
	*/
	bool is_complete(state& state){
	    if(!state.minsum)
		return false;
	    node *n = &state.top;
	    for(uint i = 0; i < TARGET_DEPTH; ++i){
		if(n->children.size() != 1)
		    return false;
		n = &n->children.front();
	    }
	    assert(n->children.size() == 0);
	    return true;
	}
    }
    std::string problem60(){
	using namespace euler60;
	state state;
	state.prime = 3;
	prime_iterator iter(6);
	while(!is_complete(state)){
	    make_matchmap(state.prime, state.match);
	    insert_into_node(state.top, state.minsum, 0, state);
	    state.prime = *iter++;
	    if(state.minsum)
		refilter(state.top, state.prime, 0, state.minsum);
	}
	return std::to_string(state.minsum);
    }
#define P(x) {x, &problem ## x}
    std::list<problem> set6
    {{P(60)}};
}
