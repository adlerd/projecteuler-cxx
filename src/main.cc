// Copyright 2014 David Adler

#include <stdexcept>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <cassert>
#include <regex>

#ifndef NO_THREADS
#include <mutex>
#include <condition_variable>
#include <thread>
#endif

#include "util.hh"
#include "atkin.hh"

using namespace euler;

u64 constexpr THREAD_COUNT = 5;

struct problem_harness {
    problem const *runner;
    u32 number;
    std::string result;
    bool required;
    bool queued;
};

struct problem_table {
    std::deque<problem_harness> items;
    std::map<u32,problem_harness*> map;
    std::list<problem_harness*> order;
    problem_harness& add(u32 number, bool required = true){
	problem_harness* ph = map[number];
	if(ph == nullptr){
	    items.push_back({nullptr,number,"",required,false});
	    ph = &items.back();
	    map[number] = ph;
	} else {
	    ph->required |= required;
	}
	order.push_back(ph);
	return *ph;
    }
};

void queue_problem(problem_harness& ph);
void unqueue_problem();
void die();
void start_threads(u32 ct);


void print_in_order(problem_table& pt){
    auto o_iter = pt.order.cbegin();
    if(o_iter == pt.order.cend())
	return;
    while(true){
	/* using threaded implementation: only the main thread uses ph.queued.
	 * The runner threads write ph.result once, but this happens-before the
	 * main thread sets ph.queued to false, which happens-before the main
	 * thread reads ph.result. */
	unqueue_problem();
	while(!(**o_iter).queued){
	    problem_harness& ph = **o_iter++;
	    std::cout << ph.number << ": " << ph.result << std::endl;
	    if(o_iter == pt.order.cend())
		return;
	}
    }
}

template <class Function>
void for_problems(Function f){
    for(auto s : sets)
	for(auto p : *s)
	    f(p);
}

u32 mtoi(char const *start, char const *const end){
    u32 s = 0;
    while(start != end){
	s *= 10;
	s += *start++ - '0';
    }
    return s;
}

void parse_selectors(int argc, char *argv[], problem_table& pt){
    std::regex rx("c(\\d+)|(\\d+)");
    std::match_results<char const*> m;
    for(int i = 1; i < argc; ++i){
	if(!std::regex_match(argv[i], m, rx))
	    throw std::invalid_argument("invalid program argument");
	if(m[1].matched){
	    u32 century = 100*mtoi(m[1].first, m[1].second);
	    for(u32 j = 0; j < 100; ++j)
		pt.add(century + j, false);
	} else {
	    pt.add(mtoi(m[2].first, m[2].second));
	}
    }
}

int main(int argc, char *argv[]){
    problem_table pt;
    if(argc == 1){
	start_threads(THREAD_COUNT);
	for_problems([&pt](problem const *p)
		{ problem_harness& ph = pt.add(p->get_number());
		  ph.runner = p; });
    } else {
	parse_selectors(argc, argv, pt);
	for_problems([&pt](problem const *p)
	      { decltype(pt.map.cend()) iter = pt.map.find(p->get_number());
	        if(iter != pt.map.cend())
		    iter->second->runner = p;
	      });
	start_threads(std::min(THREAD_COUNT, pt.items.size()));
    }
    auto iter = pt.order.begin();
    while(iter != pt.order.end()){
	auto place = iter;
	++iter;
	problem_harness& ph = **place;
	if(ph.queued) {
	} else if(ph.runner == nullptr){
	    if(ph.required){
		std::cerr << "could not find problem " << ph.number <<
		    std::endl;
		return 5;
	    }
	    pt.order.erase(place);
	} else {
	    queue_problem(ph);
	}
    }
    print_in_order(pt);
    die();
    for_problems([](problem const *p) { delete p; });
    return 0;
}

std::queue<problem_harness *, std::list<problem_harness*>> problem_queue;

#ifndef NO_THREADS

bool die_flag = false;

std::mutex done_mutex;
std::condition_variable done_condition;
std::queue<problem_harness *, std::list<problem_harness *>> done_queue;

std::mutex problem_mutex;
std::condition_variable problem_condition;

void queue_problem(problem_harness& ph){
    ph.queued = true;
    std::unique_lock<std::mutex> lock(problem_mutex);
    problem_queue.push(&ph);
    problem_condition.notify_one();
}
void unqueue_problem(){
    std::unique_lock<std::mutex> lock(done_mutex);
    while(done_queue.empty())
	done_condition.wait(lock);
    assert(lock.owns_lock());
    problem_harness& ph = *done_queue.front();
    done_queue.pop();
    ph.queued = false;
}

problem_harness *select_problem(){
    std::unique_lock<std::mutex> lock(problem_mutex);
    while(problem_queue.empty()){
	if(die_flag)
	    return nullptr;
	problem_condition.wait(lock);
    }
    problem_harness *const p = problem_queue.front();
    problem_queue.pop();
    return p;
}
void problems_runner(){
    while(true){
	problem_harness *const ph = select_problem();
	if(ph == nullptr)
	    return;
	ph->result = ph->runner->run();
	std::unique_lock<std::mutex> lock(done_mutex);
	assert(lock.owns_lock());
	done_queue.push(ph);
	done_condition.notify_all();
    }
}

std::list<std::thread*> threads;

void die(){
    {
	std::unique_lock<std::mutex> lock(problem_mutex);
	assert(problem_queue.empty());
	die_flag = true;
	problem_condition.notify_all();
    }
    for(auto t : threads){
	t->join();
	delete t;
    }
    assert(done_queue.empty());
}

void start_threads(u32 ct){
    for(u32 i = 0; i < ct; ++i)
	threads.push_back(new std::thread(&problems_runner));
}

#else

void queue_problem(problem_harness& ph){
    problem_queue.push(&ph);
    ph.queued = true;
}
void unqueue_problem(){
    problem_harness& ph = *problem_queue.front();
    problem_queue.pop();
    ph.result = ph.runner->run();
    ph.queued = false;
}

void die(){
    assert(problem_queue.empty());
}
void start_threads(u32){}

#endif
