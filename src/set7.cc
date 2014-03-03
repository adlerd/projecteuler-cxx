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
    bool is_digit_permutation(u64 a, u64 b){
	std::vector<u8> digs_a(digit_iterator(a), digit_iterator(0));
	std::vector<u8> digs_b(digit_iterator(b), digit_iterator(0));
	return digs_a.size() == digs_b.size() &&
	    std::is_permutation(digs_a.begin(), digs_a.end(), digs_b.begin());
    }
    u64 problem70(){
	u32 constexpr LIMIT = 10000000;
	double min_ratio = 87109.0 / 79180;
	u64 min_n = 87109;
	std::vector<u64> totients;
	totients.reserve(LIMIT);
	totients.push_back(1);
	totients.push_back(1);
	totients.push_back(1);
	for(u64 n = 3; n < LIMIT; ++n){
	    // find first factor
	    u32 p;
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
	    u64 tot = p-1;
	    u32 nn = n / p;
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
    u32 problem71(){
	u32 target_n = 3;
	u32 target_d = 7;
	u32 max_d = 1000000;
	u32 best_n = target_n - 1;
	u32 best_d = target_d;
	{
	    u32 g = gcd(best_n, best_d);
	    best_n /= g;
	    best_d /= g;
	}
	for(u32 d = 2; d <= max_d; ++d){
	    u32 n = d * target_n / target_d;
	    if(gcd(n, d) == 1 && n*best_d > best_n * d){
		if(d == target_d)
		    continue;
		best_d = d;
		best_n = n;
	    }
	}
	return best_n;
    }
    u64 problem72(){
	u64 sum = 0;
	for(u32 i = 2; i <= 1000000; ++i)
	    sum += totient(i);
	return sum;
    }
    u32 problem73(){
	u32 ct = 0;
	for(u32 d = 1; d <= 12000; ++d){
	    u32 n = d/3;
	    while(2*++n < d)
		if(gcd(n,d) == 1)
		    ++ct;
	}
	return ct;
    }
    u32 problem74(){
	std::vector<u8> lens(2178000);
	std::array<u32, 10> facts;
	facts[0] = 1;
	for(u32 i = 1, n = 1; i < 10; ++i)
	    facts[i] = (n *= i);
	lens[169] = lens[363601] = lens[1545] = 3;
	lens[871] = lens[45361] = 2;
	lens[872] = lens[45362] = 2;
	u32 ct = 0;
	for(u32 i = 1; i < 1000000; ++i){
	    u32 n = i;
	    u32 l = lens[n];
	    std::forward_list<u32> stack;
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
    u32 problem75(){
	class tset {
	    u32 a;
	    u32 b;
	public:
	    tset() : a(0), b(0) {}
	    tset(tset const&) = default;
	    tset(tset&&) = default;
	    void add(u32 x, u32 y){
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
	u32 constexpr limit = 1500000;
	//std::array<tset,limit> zom;
	std::vector<tset> zom(limit);
	for(u32 m = 1; m < 867; ++m){
	    for(u32 n = 1; n < m; ++n){
		u32 const p = 2*m*(n+m);
		if(p > limit)
		    break;
		if(gcd(m,n) == 1){
		    u32 x = m*m-n*n;
		    u32 y = 2*m*n;
		    if(x > y)
			std::swap(x,y);
		    u32 pp = p;
		    u32 xx = x;
		    u32 yy = y;
		    do {
			zom[pp-1].add(xx,yy);
			pp+=p;
			xx+=x;
			yy+=y;
		    } while(pp <= limit);
		}
	    }
	}
	u32 ct = 0;
	for(auto& z : zom){
	    if(z.one())
		++ct;
	}
	return ct;
    }

    class partitions {
	std::vector<u32> memo;
	u32 size;
	static u32 trinum(u32 n){
	    return (n*(n+1))/2;
	}
	u32 parts(u32 n, u32 k);
	typedef bool (* const validt)(u64);
    public:
	explicit partitions(validt v) : memo(0), size(0), valid(v) {}
	partitions(partitions const&) = default;
	partitions(partitions&&) = default;
	validt valid;
	u32 operator[](u32 i);
    };
    u32 partitions::operator[](u32 i){
	if(i >= size){
	    size = i+1;
	    memo.resize(trinum(size));
	}
	return parts(i, 1);
    }
    u32 partitions::parts(u32 n, u32 k){
	if(k > n)
	    return 0;
	u32& a = memo[trinum(n)+k];
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

    bool always_true(u64){
	return true;
    }
    u32 problem76(){
	return partitions(&always_true)[100]-1;
    }
    u32 problem77(){
	u32 x = 11;
	partitions p(&is_prime);
	while(p[x] <= 5000)
	    ++x;
	return x;
    }
    u32 problem78(){
	using namespace figurate;
	u32 constexpr lim = 1000000;
	std::vector<u32> memo(60000);
	u32 n = 1;
	memo[0] = 1;
	do {
	    assert(n < memo.size());
	    pent_iter pi;
	    u32 x = 1;
	    u32 summod = 0;
	    while(true){
		u32 i = *pi++;
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
    u32 problem79(){
	std::array<std::bitset<10>,10> follows;
	u32 out = 0;
	std::bitset<10> found;
	for(auto i = input79; *i; i += 3){
	    assert(i - input79 < 150);
	    assert('0' <= i[0] && i[0] <= '9');
	    assert('0' <= i[1] && i[1] <= '9');
	    assert('0' <= i[2] && i[2] <= '9');
	    u32 x = i[0]-'0';
	    u32 y = i[1]-'0';
	    u32 z = i[2]-'0';
	    follows[y][x] = true;
	    follows[z][y] = true;
	    follows[z][x] = true;
	    found[x] = true;
	    found[y] = true;
	    found[z] = true;
	}
	for(u32 i = 0; i < 10; ++i)
	    if(!found[i])
		follows[i][i] = true;
	u32 i = 0;
	while(i < 10){
	   if(follows[i].none()){
	       found.reset();
	       found[i] = true;
	       follows[i] = found;
	       found.flip();
	       for(u32 j = 0; j < i; ++j)
		   follows[j] &= found;
	       for(u32 j = i+1; j < 10; ++j)
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
