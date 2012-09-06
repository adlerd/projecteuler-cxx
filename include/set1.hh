#ifndef _SET1_HH_
#define _SET1_HH_

#include "util.hh"
#include <list>

namespace euler {
    extern std::list<problem const*> set1;
    namespace tri_collapse {
	ulong triangle_collapse(uchar const *start, ulong width);
    }
}

#endif
