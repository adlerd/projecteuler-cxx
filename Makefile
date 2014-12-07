# Copyright 2014 David Adler

EXTRA_FLAGS=--std=c++11 -Werror -stdlib=libc++
ALL_CPPFLAGS=-I./include/ -I./gmp
ifneq "$(NO_THREADS)" "true"
    EXTRA_FLAGS += -pthread
else
    ALL_CPPFLAGS += -DNO_THREADS
endif

CC=/usr/bin/clang
CXX=/usr/bin/clang++

LDLIBS=-L./gmp -lgmpxx -lgmp
ALL_CPPFLAGS += $(CPPFLAGS)
OBJDUMP_FLAGS=-dCSr

empty:=
space:= $(empty) $(empty)

libsources = atkin.cc util.cc algx.cc
setsources = set0.cc set1.cc set2.cc set3.cc set4.cc set5.cc set6.cc set7.cc \
	     set8.cc set9.cc set10.cc
sources = $(libsources) $(setsources) main.cc

SETS = $(patsubst set%.cc,%,$(setsources))

vpath %.cc src/
vpath %.hh include/

default: bin/euler

libs: $(subst .cc,.o,$(libsources))

clean:
	rm -f *.?ps
	rm -f *.o
	rm -rf bin
	rm -f sets.cc.gen

cleanall: clean
	rm -f *.d
	rm -f *.d.????

%.d: %.cc Makefile
	$(CC) $(EXTRA_FLAGS) -MM $(ALL_CPPFLAGS) $< > $@

include $(if $(if $(MAKECMDGOALS),$(filter-out clean cleanall,$(MAKECMDGOALS)),"x"),$(sources:%.cc=%.d))
#doesn't include *.d if MAKECMDGOALS consists *only* of clean and cleanall

%.o: %.cc gmp/gmpxx.h
	$(CXX) $(EXTRA_FLAGS) -o $@ $< -c $(ALL_CPPFLAGS) $(CXXFLAGS) 2>&1

sets.cc.gen::
	src/sets.cc.zsh $(SETS) > $@

sets.o: sets.cc.gen
	$(CXX) $(EXTRA_FLAGS) -o $@ -x 'c++' $< -c $(ALL_CPPFLAGS) $(CXXFLAGS) 2>&1

bin/euler: $(subst .cc,.o,$(libsources)) main.o $(patsubst %,set%.o,$(SETS)) sets.o \
    | gmp/libgmp.a gmp/libgmpxx.a
	mkdir -p bin
	$(CXX) $(EXTRA_FLAGS) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) 2>&1

%.cps: %.cc
	$(CXX) $(EXTRA_FLAGS) -S $(ALL_CPPFLAGS) $(CXXFLAGS) $< -o $@

%.dps: %.o
	objdump $(OBJDUMP_FLAGS) $< > $@

%.pps: %.cc
	$(CC) $(EXTRA_FLAGS) -E $(ALL_CPPFLAGS) $< -o $@

%.hps: %.hh
	$(CC) $(EXTRA_FLAGS) -E $(ALL_CPPFLAGS) $< -o $@

gmp-5.1.3.txz:
	curl 'https://ftp.gnu.org/gnu/gmp/gmp-5.1.3.tar.xz' > $@

gmp/gmpxx.h gmp/gmp.h gmp/libgmp.a gmp/libgmpxx.a: gmp

gmp: gmp-5.1.3.txz
	mkdir -p gmp-build-tmp/build
	xzcat $< | tar -x -C gmp-build-tmp
	cd gmp-build-tmp/build && ../gmp-5.1.3/configure ABI=64 CC="$(CC)" CXX="$(CXX) -stdlib=libc++" CFLAGS="-O2" --enable-cxx --disable-shared --prefix=$(CURDIR)/gmp-build-tmp/install
	cd gmp-build-tmp/build && $(MAKE)
	cd gmp-build-tmp/build && $(MAKE) install
	mkdir -p gmp
	cd gmp-build-tmp/install && mv include/gmp.h include/gmpxx.h lib/libgmp.a lib/libgmpxx.a ../../gmp
	rm -rf gmp-build-tmp
