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

ulong constexpr THREAD_COUNT = 5;

void queue_problem(problem const *fun);
answer retrieve_answer();
void die();
void start_threads(uint ct);

struct problem_harness {
    problem const *runner;
    uint number;
    std::string result;
    bool required;
    bool queued;
};

struct problem_table {
    std::deque<problem_harness> items;
    std::map<uint,problem_harness*> map;
    std::list<problem_harness*> order;
    problem_harness& add(uint number, bool required = true){
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

void print_in_order(problem_table& pt){
    auto o_iter = pt.order.cbegin();
    if(o_iter == pt.order.cend())
	return;
    while(true){
	{
	    answer a = retrieve_answer();
	    problem_harness& ph = *pt.map[a.first];
	    assert(ph.queued);
	    ph.result = a.second;
	    ph.queued = false;
	}
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

uint mtoi(char const *start, char const *const end){
    uint s = 0;
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
	    uint century = 100*mtoi(m[1].first, m[1].second);
	    for(uint j = 0; j < 100; ++j)
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
	    queue_problem(ph.runner);
	    ph.queued = true;
	}
    }
    print_in_order(pt);
    die();
    for_problems([](problem const *p) { delete p; });
    return 0;
}

std::queue<problem const*, std::list<problem const*>> problem_queue;

#ifndef NO_THREADS

bool die_flag = false;

std::mutex answer_mutex;
std::condition_variable answer_condition;
std::queue<answer, std::list<answer>> answer_queue;

std::mutex problem_mutex;
std::condition_variable problem_condition;

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

void start_threads(uint ct){
    for(uint i = 0; i < ct; ++i)
	threads.push_back(new std::thread(&problems_runner));
}

#else

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
void start_threads(uint){}

#endif
