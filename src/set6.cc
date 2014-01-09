#include "set6.hh"
#include "set1.hh"
#include "atkin.hh"
#include <map>
#include <list>
#include <forward_list>
#include <algorithm>
#include <array>
#include <set>
#include <bitset>
#include <iterator>

namespace {
    using namespace euler;
    namespace euler60 {
	struct pdef {
	    // it's just std::pair, but needs a name to avoid infinite type
	    ulong prime;
	    std::vector<pdef const*> m;
	};
	typedef std::vector<pdef> store;
	typedef std::list<pdef const*> matchlist;
	store make_primes(){
	    store ret;
	    ret.push_back(pdef{3});
	    for(prime_iterator pi(6); *pi < 10000; ++pi)
		ret.push_back(pdef{*pi});
	    return ret;
	}
	void make_matchmap(store& info){
	    decltype(pdef::m) tmp;
	    uint const size = info.size();
	    ulong order_i = 10;
	    for(uint i = 0; i < size; ++i){
		ulong const pi = info[i].prime;
		if(order_i <= pi)
		    order_i *= 10;
		ulong order_j = order_i;
		for(uint j =  i + 1; j < size; ++j){
		    ulong const pj = info[j].prime;
		    if(order_j <= pj)
			order_j *= 10;
		    if(is_prime(pi + order_i * pj) && is_prime(pj + order_j * pi))
			tmp.push_back(&info[j]);
		}
		tmp.shrink_to_fit();
		std::swap(tmp, info[i].m);
	    }
	}
	bool compare_pdef(pdef const* a, pdef const* b){
	    return a->prime < b->prime;
	}
	ulong rec(matchlist const& matches, uint const n){
	    uint const req = 5 - n;
	    if(matches.size() < req)
		return 0;
	    if(n == 4)
		return matches.front()->prime;
	    auto end = matches.cend();
	    std::advance(end, -static_cast<int>(req - 1));
	    auto iter = matches.cbegin();
	    while(iter != end){
		matchlist out;
		auto val = *iter++;
		std::set_intersection(iter, matches.cend(),
			val->m.cbegin(), val->m.cend(),
			std::back_insert_iterator<matchlist>(out),&compare_pdef);
		ulong res = rec(out,n+1);
		if(res)
		    return res + val->prime;
	    }
	    return 0;
	}
    }
    ulong problem60(){
	using namespace euler60;
	store store = make_primes();
	make_matchmap(store);
	matchlist master;
	for(pdef const& p : store)
	    master.push_back(&p);
	ulong result = rec(master,0);
	if(result == 0)
	    throw std::logic_error("problem 60 unsolved");
	return result;
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
    ulong problem65(){
	typedef mpq_class bigq;
	bigq part = 1;
	for(uint i = 99; i > 0; --i){
	    part = 1 / part;
	    if(i % 3 == 0)
		part += 2 * (i / 3);
	    else
		part += 1;
	}
	part += 1;
	return digit_sum(part.get_num());
    }
    namespace euler66 {
	struct basis {
	    ulong n;
	    ulong w_0;
	    explicit basis(ulong nn) : n(nn), w_0(isqrt_part(nn)) {}
	};
	struct term {
	    bigint prev_n, prev_d;
	    bigint n, d;
	    ulong w_n, d_n, m_n;
	    term next(basis const& b) const {
		ulong next_m = d_n * w_n - m_n;
		ulong next_d = (b.n - next_m * next_m) / d_n;
		assert(next_d * d_n == b.n - next_m * next_m);
		ulong next_w = (b.w_0 + next_m) / next_d;
		return {n, d, w_n * n + prev_n, w_n * d + prev_d,
		    next_w, next_d, next_m};
	    }
	};
	bigint min_x(uint d){
	    basis const b(d);
	    term t = { 0, 1, 1, 0, b.w_0, 1, 0 };
	    t = t.next(b);
	    while(true){
		t = t.next(b);
		if(t.n * t.n - d * t.d * t.d == 1)
		    return t.n;
	    }
	}
    }
    uint problem66(){
	bigint max_x = 0;
	uint res_d = 0;
	for(uint d = 2; d < 1001; ++d){
	    if(isqrt(d) == 0){
		bigint x = euler66::min_x(d);
		if(x > max_x){
		    max_x = x;
		    res_d = d;
		}
	    }
	}
	return res_d;
    }
    std::array<uchar const, 5050> const input67 = {{
#include "bigtriangle.include"
    }};
    ulong problem67(){
	return triangle_collapse(input67.end(), 100);
    }
    namespace euler68 {
	typedef std::array<uchar, 10> ring;
	typedef std::bitset<9> dset;
	ulong concat_ulong(ulong a, ulong b){
	    ulong t  = 1;
	    while(t <= b)
		t *= 10;
	    return a*t + b;
	}
	ulong to_seq(ring& ring){
	    uint mi = 0;
	    uchar md = 10;
	    for(uint i = 0; i < ring.size(); i += 2){
		if(ring[i] < md){
		    mi = i;
		    md = ring[i];
		}
	    }
	    ulong concat = 0;
	    for(uint i = mi; i < 10; i += 2){
		concat = concat_ulong(concat, ring[i]);
		concat = concat_ulong(concat, ring[i+1]);
		concat = concat_ulong(concat, ring[(i+3)%10]);
	    }
	    for(uint i = 0; i < mi; i += 2){
		concat = concat_ulong(concat, ring[i]);
		concat = concat_ulong(concat, ring[i+1]);
		concat = concat_ulong(concat, ring[(i+3)%10]);
	    }
	    return concat;
	}
	template <uchar layer>
	void rec(dset& digs, ring& ring, ulong& best) noexcept {
	    switch(layer){
	    case 10:
		if(ring[0] + ring[3] != ring[8] + ring[9])
		    break;
		assert(digs.all());
		best = std::max(best, to_seq(ring));
		break;
	    case 0:
		ring[0] = 10;
		rec<1>(digs, ring, best);
		break;
	    case 5:
	    case 7:
	    case 9:
		{
		    uchar const base = ring[0] + ring[1] + ring[3];
		    uchar o = ring[layer-2] + ring[layer-3];
		    if(o >= base)
			break;
		    o = base - o - 1;
		    if(o > 8 || digs[o])
			break;
		    digs[o] = true;
		    ring[layer] = o + 1;
		    rec<layer+1>(digs, ring, best);
		    digs[o] = false;
		    break;
		}
	    default:
		for(int i = 0; i < 9; ++i){
		    if(digs[i])
			continue;
		    else {
			digs[i] = true;
			ring[layer] = i + 1;
			rec<layer+1>(digs, ring, best);
			digs[i] = false;
		    }
		}
		break;
	    }
	}
	template <>
	void rec<11>(dset& digs, ring& ring, ulong& best) noexcept {
	    std::logic_error("LOGIC ERROR.");
	}
    }
    ulong problem68(){
	using namespace euler68;
	dset digs;
	ring ring;
	ulong best = 0;
	rec<0>(digs, ring, best);
	return best;
    }
    ulong problem69(){
	float mr = 0.0;
	ulong mn = 0;
	for(ulong i = 2; i <= 1000000; ++i){
	    float ratio = ((float)i) / totient(i);
	    if(ratio > mr){
		mr = ratio;
		mn = i;
	    }
	}
	return mn;
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set6
    {{P(60),P(61),P(62),P(63),P(64),P(65),P(66),P(67),P(68),P(69)}};
}
