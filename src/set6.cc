#include "set6.hh"
#include "atkin.hh"
#include <map>
#include <list>
#include <forward_list>
#include <algorithm>
#include <array>
#include <set>

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
    ulong problem60(){
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
	return state.minsum;
    }
    namespace euler61 {
	struct fourd {
	    uchar top;
	    uchar bottom;
	    fourd(ulong a) : top(a / 100), bottom(a % 100) {
		assert(top < 100);
	    }
	    fourd(uchar t, uchar b) : top(t), bottom(b) {
		assert(t < 100 && b < 100);
	    }

	    fourd(fourd&&) = default;
	    fourd(fourd const&) = default;
	    ~fourd() = default;

	    operator ulong(){
		return top * 100 + bottom;
	    }
	    bool operator==(fourd const& other) const {
		return (top == other.top) && (bottom == other.bottom);
	    }
	};
	typedef std::vector<fourd> figvec;
	typedef std::list<figvec*> veclist;
	typedef std::forward_list<fourd> resultlist;
	template <uint sides>
	void populate_vector(figvec& vec){
	    for(auto iter = figurate_iterator<sides>::at_least(1000);
		    *iter < 10000; ++iter)
		vec.emplace_back(*iter);
	}
	std::unique_ptr<resultlist> rec(veclist vecs, fourd prev, fourd& first){
	    assert(vecs.begin() != vecs.end());
	    figvec *alt = vecs.front();
	    vecs.pop_front();
	    if(vecs.empty()){
		assert(prev != 0);
		fourd target(prev.bottom, first.top);
		if(std::find(alt->begin(), alt->end(), target) != alt->end())
		    return std::unique_ptr<resultlist>
			(new resultlist(1, target));
	    } else if (((ulong)prev) == 0){
		for(auto& fd : *alt){
		    auto ret = rec(vecs, fd, fd);
		    if(ret != nullptr){
			ret->push_front(fd);
			return ret;
		    }
		}
	    } else {
		veclist::iterator iter = vecs.begin();
		while(true){
		    for(auto& fd : *alt){
			if(fd.top == prev.bottom){
			    auto ret = rec(vecs, fd, first);
			    if(ret != nullptr){
				ret->push_front(fd);
				return ret;
			    }
			}
		    }
		    if(iter == vecs.end())
			break;
		    std::swap(alt,*iter++);
		}
	    }
	    return nullptr;
	}
    }
    ulong problem61(){
	using namespace euler61;
	figvec vecs[6];
	populate_vector<3>(vecs[0]);
	populate_vector<4>(vecs[1]);
	populate_vector<5>(vecs[2]);
	populate_vector<6>(vecs[3]);
	populate_vector<7>(vecs[4]);
	populate_vector<8>(vecs[5]);
	veclist master;
	for(figvec *v = vecs; v < vecs+6; ++v)
	    master.push_back(v);
	fourd fake(0);
	auto sol = rec(master, 0, fake);
	if(sol == nullptr)
	    throw std::logic_error("problem 61 unsolved");
	ulong sum = 0;
	for(auto& fd : *sol)
	    sum += fd;
	return sum;
    }
    namespace euler62 {
	typedef std::array<uchar, 10> digit_cts;
	digit_cts count_digits(bigint& n){
	    digit_cts counts = {{0,0,0,0,0,0,0,0,0,0}};
	    big_digit_iterator iter(n);
	    while(iter != big_digit_iterator())
		counts[*iter++]++;
	    return counts;
	}
	struct count_entry {
	    bigint lowest;
	    uchar ct;
	};
    }
    bigint problem62(){
	using namespace euler62;
	std::map<digit_cts, count_entry> counts;
	for(ulong n = 4; true; ++n){
	    bigint cube = ((bigint)n) * n * n;
	    auto cts = count_digits(cube);
	    auto iter = counts.find(cts);
	    if(iter != counts.end()){
		if(++(iter->second.ct) == 5)
		    return iter->second.lowest;
	    } else {
		counts.insert({cts, {cube, 1}});
	    }
	}
    }
    ulong problem63(){
	ulong ct = 0;
	for(uint b = 1; b < 10; ++b){
	    bigint pow = b;
	    uint e = 1;
	    while(distance(big_digit_iterator(pow), {}) == e){
		pow *= b;
		++e;
	    }
	    ct += e-1;
	}
	return ct;
    }
    namespace euler64 {
	typedef std::array<uint, 3> triple;
	typedef std::set<triple> tset;
	ulong period(ulong n){
	    uint abs_floor = isqrt_part(n);
	    if(abs_floor * abs_floor == n)
		return 0;
	    triple prev = {{ abs_floor, 0, 1}};
	    triple t;
	    tset set;
	    while(true){
		ulong m = t[1] = prev[2] * prev[0] - prev[1];
		ulong d = t[2] = (n - m * m) / prev[2];
		assert((n - m * m) % prev[2] == 0);
		t[0] = (abs_floor + m) / d;
		if(set.find(t) != set.end())
		    return set.size();
		set.insert(t);
		std::copy(t.begin(), t.end(), prev.begin());
	    }
	}
    }
    ulong problem64(){
	ulong ct = 0;
	for(ulong n = 1; n <= 10000; ++n)
	    if(euler64::period(n) % 2 != 0)
		++ct;
	return ct;
    }
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set6
    {{P(60),P(61),P(62),P(63),P(64)}};
}
