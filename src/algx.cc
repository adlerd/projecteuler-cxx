#include "algx.hh"

#include <cassert>

using namespace euler;

struct euler::algx_impl {
    typedef algx_state::elt elt;
    static void unlink_ud(elt const e){
	e->up->down = e->down;
	e->down->up = e->up;
	e->header->size--;
    }
    static void unlink_lr(elt const e){
	e->left->right = e->right;
	e->right->left = e->left;
    }
    static void relink_ud(elt const e){
	e->up->down = e;
	e->down->up = e;
	e->header->size++;
    }
    static void relink_lr(elt const e){
	e->left->right = e;
	e->right->left = e;
    }
    /* unlink the row of first from every column except the column of first */
    static void unlink_row(elt const first){
	elt e = first->right;
	while(e != first){
	    unlink_ud(e);
	    e = e->right;
	}
    }
    /* unlink every row in the column of head, and head itself */
    static void unlink_column(elt const head){
	unlink_lr(head);
	elt e = head->down;
	while(e != head){
	    unlink_row(e);
	    e = e->down;
	}
    }
    /* undo unlink_row */
    static void relink_row(elt const first){
	elt e = first->right;
	while(e != first){
	    relink_ud(e);
	    e = e->right;
	}
    }
    /* undo unlink_column */
    static void relink_column(elt const head){
	relink_lr(head);
	elt e = head->up;
	while(e != head){
	    relink_row(e);
	    e = e->up;
	}
    }
    static void insert(elt const e, elt const up, elt const left){
	e->left = left;
	e->right = left->right;
	e->up = up;
	e->down = up->down;
	e->header = up->header;
	relink_ud(e);
	relink_lr(e);
	e->size = 0;
    }
    static void insert_key(elt const head, elt const fresh, u32 const key,
	    bool on_left_end){
	typedef algx_state::element ee;
	elt ee::*const dir = on_left_end ? &ee::left : &ee::right;
	// all pointers using *dir are to the row-header column
	// and all references to 'size' are actually to row id in row header
	elt up = head->up;
	while(up != head && (up->*dir)->size > key)
	    up = up->up;
	assert(up == head || (up->*dir)->size != key);

	elt row_head = (up->*dir)->down;
	while(row_head->size != key){
	    assert(row_head->header != row_head);
	    assert(row_head->size < key);
	    row_head = row_head->down;
	}
	insert(fresh, up, on_left_end ? row_head : row_head->left);
    }
};

algx_state::algx_state(u32 rowcount) : store(rowcount+1),
    origin(&store.front()), state() {
    origin->left = origin->right = origin;
    for(u32 i = 0; i < rowcount; ++i){
	store[i].down = &store[i+1];
	store[i+1].up = &store[i];
	store[i+1].header = origin;
	store[i+1].size = i;
	store[i+1].left = store[i+1].right = &store[i+1];
    }
    store[0].up = &store[rowcount];
    store[rowcount].down = &store[0];
    origin->header = origin;
    origin->optional = true;
}

void algx_state::push_required(){
    assert(is_clean());
    store.emplace_back();
    elt const e = &store.back();
    e->header = e;
    algx_impl::insert(e, e, origin);
}
void algx_state::push_optional(){
    assert(is_clean());
    store.emplace_front();
    elt const e = &store.front();
    e->optional = true;
    e->header = e;
    algx_impl::insert(e, e, origin->left);
}
void algx_state::pop_required(){
    assert(is_clean());
    elt const header = store.back().header;
    u32 const size = header->size;
    for(u32 i = 0; i <= size; ++i){
	elt const e = &store.back();
	assert(i == size ? e == header : true);
	algx_impl::unlink_lr(e);
	store.pop_back();
    }
}
void algx_state::pop_optional(){
    assert(is_clean());
    elt const header = store.front().header;
    u32 const size = header->size;
    for(u32 i = 0; i <= size; ++i){
	elt const e = &store.front();
	assert(i == size ? e == header : true);
	algx_impl::unlink_lr(e);
	store.pop_front();
    }
}
void algx_state::add_required_entry(u32 key){
    assert(is_clean());
    elt const header = store.back().header;
    store.emplace_back();
    algx_impl::insert_key(header, &store.back(), key, true);
}
void algx_state::add_optional_entry(u32 key){
    assert(is_clean());
    elt const header = store.front().header;
    store.emplace_front();
    algx_impl::insert_key(header, &store.front(), key, false);
}

std::vector<u32> algx_state::read_solution() const {
    assert(!is_clean());
    std::vector<u32> ret;
    for(elt e : state){
	while(e->header != origin)
	    e = e->right;
	ret.push_back(e->size);
    }
    return ret;
}

void algx_state::clean_one(){
    elt const r = state.front()->left;
    elt j = r;
    do {
	if(j->header != origin)
	    algx_impl::relink_column(j->header);
	j = j->left;
    } while(j != r);
}
/* prepare for [tail-]recursion to search_down, and do the actual deletions */
bool algx_state::unclean_one(){
    elt const r = state.front();
    elt j = r;
    do {
	if(j->header != origin)
	    algx_impl::unlink_column(j->header);
	j = j->right;
    } while(j != r);
    return search_down();
}
/* perform the non-deterministic selection of r */
bool algx_state::search_up(){
    clean_one();
    elt const r = state.front() = state.front()->down;
    if(r == r->header){
	state.pop_front();
	if(is_clean())
	    return false;
	else
	    return search_up();
    } else {
	return unclean_one();
    }
}
/* check for solution and push new solution candidates */
bool algx_state::search_down(){
    if(origin->right->optional){
	/* origin->optional is set so that if we've eliminated all optional as
	 * well we still see success.
	 * return !is_clean() instead of success so we don't return true on
	 * empty state */
	return !is_clean();
    } else {
	elt minc = origin->right;
	u32 minsize = minc->size;
	if(minsize >= 2){
	    elt c = minc->right;
	    /* never select an optional, it's not productive */
	    while(!c->optional){
		if(c->size < minsize){
		    minc = c;
		    minsize = c->size;
		    /* the idea is that if we find a 0 we need to go ahead and
		     * use it to fail quick... but if we find a 1 first, use
		     * that instead so we can eliminate a bunch of 1s cheaply
		     * without searching for 0s first */
		    if(c->size < 2)
			break;
		}
		c = c->right;
	    }
	}
	if(minsize == 0){
	    if(is_clean())
		return false; // no solutions whatsoever!
	    else
		return search_up();
	} else {
	    assert(minc->down != minc);
	    state.push_front(minc->down);
	    return unclean_one();
	}
    }
}
