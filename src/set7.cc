#include "atkin.hh"
#include "util.hh"
#include <list>
#include <forward_list>
#include <array>
#include <algorithm>
#include <cassert>
#include <bitset>

namespace {
    using namespace euler;
    bool is_digit_permutation(ulong a, ulong b){
	std::vector<uchar> digs_a(digit_iterator(a), digit_iterator(0));
	std::vector<uchar> digs_b(digit_iterator(b), digit_iterator(0));
	return digs_a.size() == digs_b.size() &&
	    std::is_permutation(digs_a.begin(), digs_a.end(), digs_b.begin());
    }
    ulong problem70(){
	uint constexpr LIMIT = 10000000;
	double min_ratio = 87109.0 / 79180;
	ulong min_n = 87109;
	std::vector<ulong> totients;
	totients.reserve(LIMIT);
	totients.push_back(1);
	totients.push_back(1);
	totients.push_back(1);
	for(ulong n = 3; n < LIMIT; ++n){
	    // find first factor
	    uint p;
	    for(prime_iterator pi; ; ++pi){
		p = *pi;
		if(n % p == 0)
		    break;
		if(p*p > n){
		    p = n;
		    break;
		}
	    }
	    // now calculate the totient of just the powers of p in n
	    ulong tot = p-1;
	    uint nn = n / p;
	    while(nn % p == 0){
		nn /= p;
		tot *= p;
	    }
	    // and use the fact that totient(n) is multiplicative
	    tot *= totients[nn];
	    // store in totients[n]
	    totients.push_back(tot);

	    double ratio = ((double) n) / tot;
	    if(ratio < min_ratio && is_digit_permutation(n,tot)){
		min_ratio = ratio;
		min_n = n;
	    }
	}
	return min_n;
    }
    uint problem71(){
	uint target_n = 3;
	uint target_d = 7;
	uint max_d = 1000000;
	uint best_n = target_n - 1;
	uint best_d = target_d;
	{
	    uint g = gcd(best_n, best_d);
	    best_n /= g;
	    best_d /= g;
	}
	for(uint d = 2; d <= max_d; ++d){
	    uint n = d * target_n / target_d;
	    if(gcd(n, d) == 1 && n*best_d > best_n * d){
		if(d == target_d)
		    continue;
		best_d = d;
		best_n = n;
	    }
	}
	return best_n;
    }
    ulong problem72(){
	ulong sum = 0;
	for(uint i = 2; i <= 1000000; ++i)
	    sum += totient(i);
	return sum;
    }
    uint problem73(){
	uint ct = 0;
	for(uint d = 1; d <= 12000; ++d){
	    uint n = d/3;
	    while(2*++n < d)
		if(gcd(n,d) == 1)
		    ++ct;
	}
	return ct;
    }
    uint problem74(){
	std::vector<uchar> lens(2178000);
	std::array<uint, 10> facts;
	facts[0] = 1;
	for(uint i = 1, n = 1; i < 10; ++i)
	    facts[i] = (n *= i);
	lens[169] = lens[363601] = lens[1545] = 3;
	lens[871] = lens[45361] = 2;
	lens[872] = lens[45362] = 2;
	uint ct = 0;
	for(uint i = 1; i < 1000000; ++i){
	    uint n = i;
	    uint l = lens[n];
	    std::forward_list<uint> stack;
	    while(l == 0){
		stack.push_front(n);
		digit_iterator di(n);
		n = 0;
		while(di != digit_iterator(0))
		    n += facts[*di++];
		if(n == stack.front()){
		    l = 1;
		} else if(lens[n]){
		    if(lens[n] >= 60)
			l = 61;
		    else
			l = lens[n] + 1;
		}
	    }
	    while(!stack.empty()){
		lens[stack.front()] = l;
		stack.pop_front();
		if(l == 60)
		    ++ct;
		if(l < 61)
		    ++l;
	    }
	}
	return ct;
    }
    uint problem75(){
	class tset {
	    uint a;
	    uint b;
	public:
	    tset() : a(0), b(0) {}
	    tset(tset const&) = default;
	    tset(tset&&) = default;
	    void add(uint x, uint y){
		if(a == 0){
		    if(b == 0){
			a = x;
			b = y;
		    }
		} else if(!(a == x && b == y)){
		    a = 0;
		    b = 1;
		}
	    }
	    bool one(){
		return a != 0;
	    }
	};
	uint constexpr limit = 1500000;
	//std::array<tset,limit> zom;
	std::vector<tset> zom(limit);
	for(uint m = 1; m < 867; ++m){
	    for(uint n = 1; n < m; ++n){
		uint const p = 2*m*(n+m);
		if(p > limit)
		    break;
		if(gcd(m,n) == 1){
		    uint x = m*m-n*n;
		    uint y = 2*m*n;
		    if(x > y)
			std::swap(x,y);
		    uint pp = p;
		    uint xx = x;
		    uint yy = y;
		    do {
			zom[pp-1].add(xx,yy);
			pp+=p;
			xx+=x;
			yy+=y;
		    } while(pp <= limit);
		}
	    }
	}
	uint ct = 0;
	for(auto& z : zom){
	    if(z.one())
		++ct;
	}
	return ct;
    }

    class partitions {
	std::vector<uint> memo;
	uint size;
	static uint trinum(uint n){
	    return (n*(n+1))/2;
	}
	uint parts(uint n, uint k);
	typedef bool (* const validt)(ulong);
    public:
	explicit partitions(validt v) : memo(0), size(0), valid(v) {}
	partitions(partitions const&) = default;
	partitions(partitions&&) = default;
	validt valid;
	uint operator[](uint i);
    };
    uint partitions::operator[](uint i){
	if(i >= size){
	    size = i+1;
	    memo.resize(trinum(size));
	}
	return parts(i, 1);
    }
    uint partitions::parts(uint n, uint k){
	if(k > n)
	    return 0;
	uint& a = memo[trinum(n)+k];
	if(!a) {
	    if(k == n)
		a = (*valid)(n) ? 1 : 0;
	    else if((*valid)(k))
		a = parts(n, k+1) + parts(n-k, k);
	    else
		a = parts(n, k+1);
	}
	return a;
    }

    bool always_true(ulong){
	return true;
    }
    uint problem76(){
	return partitions(&always_true)[100]-1;
    }
    uint problem77(){
	uint x = 11;
	partitions p(&is_prime);
	while(p[x] <= 5000)
	    ++x;
	return x;
    }
    uint problem78(){
	using namespace figurate;
	uint constexpr lim = 1000000;
	std::vector<uint> memo(60000);
	uint n = 1;
	memo[0] = 1;
	do {
	    assert(n < memo.size());
	    pent_iter pi;
	    uint x = 1;
	    uint summod = 0;
	    while(true){
		uint i = *pi++;
		if(i > n)
		    break;
		summod += memo[n - i];
		i += x++;
		if(i > n)
		    break;
		summod += memo[n - i];
		if(summod < 2*lim)
		    summod += 2*lim;
		i = *pi++;
		if(i > n)
		    break;
		summod -= memo[n - i];
		i += x++;
		if(i > n)
		    break;
		summod -= memo[n - i];
		if(summod > 10*lim)
		    summod %= lim;
	    }
	    memo[n] = summod % lim;
	} while(memo[n++]);
	return n-1;
    }
    char const *const input79 = "319680180690129620762689762318368710720710629168160689716731736729316729729710769290719680318389162289162718729319790680890362319760316729380319728716";
    uint problem79(){
	std::array<std::bitset<10>,10> follows;
	uint out = 0;
	std::bitset<10> found;
	for(auto i = input79; *i; i += 3){
	    assert(i - input79 < 150);
	    assert('0' <= i[0] && i[0] <= '9');
	    assert('0' <= i[1] && i[1] <= '9');
	    assert('0' <= i[2] && i[2] <= '9');
	    uint x = i[0]-'0';
	    uint y = i[1]-'0';
	    uint z = i[2]-'0';
	    follows[y][x] = true;
	    follows[z][y] = true;
	    follows[z][x] = true;
	    found[x] = true;
	    found[y] = true;
	    found[z] = true;
	}
	for(uint i = 0; i < 10; ++i)
	    if(!found[i])
		follows[i][i] = true;
	uint i = 0;
	while(i < 10){
	   if(follows[i].none()){
	       found.reset();
	       found[i] = true;
	       follows[i] = found;
	       found.flip();
	       for(uint j = 0; j < i; ++j)
		   follows[j] &= found;
	       for(uint j = i+1; j < 10; ++j)
		   follows[j] &= found;
	       out *= 10;
	       out += i;
	       i = 0;
	   } else {
	       ++i;
	   }
	}
	return out;
    }
}
namespace euler {
#define P(x) new_problem(x, &problem ## x)
    std::list<problem const*> set7
    {{P(70),P(71),P(72),P(73),P(74),P(75),P(76),P(77),P(78),P(79)}};
}
