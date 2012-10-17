#include <stdexcept>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <cassert>

#ifndef NO_THREADS
#include <mutex>
#include <condition_variable>
#include <thread>
#endif

#include "util.hh"
#include "atkin.hh"
#include "set0.hh"
#include "set1.hh"
#include "set2.hh"
#include "set3.hh"
#include "set4.hh"
#include "set5.hh"
#include "set6.hh"
#include "set7.hh"

using namespace euler;

void queue_problem(problem const *fun);
answer retrieve_answer();
void die();
void start_threads();

void print_answer(answer const& a){
    std::cout << a.first << ": " << a.second << std::endl;
}
void print_in_order(std::vector<ulong> const& order){
    std::map<ulong, std::string> map;
    auto o_iter = order.cbegin();
    if(o_iter == order.cend())
	return;
    while(true){
	answer a = retrieve_answer();
	if(a.first == *o_iter){
	    print_answer(a);
	    while(true){
		if(++o_iter == order.cend())
		    return;
		auto const place = map.find(*o_iter);
		if(place == map.cend())
		    break;
		print_answer(*place);
		map.erase(place);
	    }
	} else {
	    map.insert(std::move(a));
	}
    }
}
template <class Function>
void for_problems(Function f){
    std::for_each(set0.cbegin(), set0.cend(), f);
    std::for_each(set1.cbegin(), set1.cend(), f);
    std::for_each(set2.cbegin(), set2.cend(), f);
    std::for_each(set3.cbegin(), set3.cend(), f);
    std::for_each(set4.cbegin(), set4.cend(), f);
    std::for_each(set5.cbegin(), set5.cend(), f);
    std::for_each(set6.cbegin(), set6.cend(), f);
    std::for_each(set7.cbegin(), set7.cend(), f);
}
int main(int argc, char* argv[]){
    start_threads();
    std::vector<ulong> order;
    if(argc == 1){
	for_problems([&order](problem const *p)
		{ order.push_back(p->get_number());
		  queue_problem(p); });
    } else {
	std::set<ulong> set;
	for(int i = 1; i < argc; ++i){
	    std::string str(argv[i]);
	    size_t idx;
	    ulong ul = stoul(str,&idx);
	    if(idx != str.size())
		throw std::invalid_argument("invalid input");
	    if(set.find(ul) == set.cend()){
		order.push_back(stoul(str,&idx));
		set.insert(ul);
	    }
	}
	for_problems([&set](problem const *p)
		{ auto const place = set.find(p->get_number());
		  if(place != set.cend()){
		      queue_problem(p);
		      set.erase(place);
		  } });
	if(!set.empty())
	    throw std::invalid_argument("Could not find problem");
    }
    print_in_order(order);
    die();
    return 0;
}

#ifndef NO_THREADS

ulong constexpr THREAD_COUNT = 5;

bool die_flag = false;

std::mutex answer_mutex;
std::condition_variable answer_condition;
std::queue<answer, std::list<answer>> answer_queue;

std::mutex problem_mutex;
std::condition_variable problem_condition;
std::queue<problem const*, std::list<problem const*>> problem_queue;

void queue_problem(problem const *problem){
    std::unique_lock<std::mutex> lock(problem_mutex);
    problem_queue.push(problem);
    problem_condition.notify_one();
}
answer retrieve_answer(){
    std::unique_lock<std::mutex> lock(answer_mutex);
    while(answer_queue.empty())
	answer_condition.wait(lock);
    assert(lock.owns_lock());
    answer ans = answer_queue.front();
    answer_queue.pop();
    return ans;
}

void problems_runner(){
    while(true){
	std::unique_lock<std::mutex> lock(problem_mutex);
	if(die_flag)
	    return;
	if(problem_queue.empty()) {
	    problem_condition.wait(lock);
	} else {
	    problem const *p = problem_queue.front();
	    problem_queue.pop();
	    lock.unlock();
	    std::string const str = p->run();
	    lock = std::unique_lock<std::mutex>(answer_mutex);
	    assert(lock.owns_lock());
	    answer_queue.push({p->get_number(), str});
	    answer_condition.notify_all();
	}
    }
}

std::list<std::thread*> threads;

void die(){
    {
	std::unique_lock<std::mutex> lock1(problem_mutex);
	assert(problem_queue.empty());
	die_flag = true;
	problem_condition.notify_all();
    }
    for(auto t : threads){
	t->join();
	delete t;
    }
    assert(answer_queue.empty());
}

void start_threads(){
    for(uint i = 0; i < THREAD_COUNT; ++i)
	threads.push_back(new std::thread(&problems_runner));
}

#else

std::queue<problem const*, std::list<problem const*>> problem_queue;

void queue_problem(problem const *problem){
    problem_queue.push(problem);
}
answer retrieve_answer(){
    problem const *p = problem_queue.front();
    problem_queue.pop();
    return {p->get_number(), p->run()};
}

void die(){
    assert(problem_queue.empty());
}
void start_threads(){}

#endif
