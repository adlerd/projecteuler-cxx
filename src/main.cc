#include <iostream>

#include "atkin.hh"
#include <thread>

int main(){
    using namespace euler;
    ulong sum = 0;
    ulong prev = 0;
    for(prime_iterator p_iter; *p_iter < 50000000; ++p_iter){
	ulong const p = *p_iter;
	sum += p;
	if(p > prev){
	    std::cout << '.' << std::flush;
	    prev = p + 1000000;
	}
    }
    std::cout << std::endl << sum << std::endl;
    return 0;
}
