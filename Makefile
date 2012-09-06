CC=/usr/bin/gcc-4.7.1 --std=c++11
CXX=/usr/bin/g++-4.7.1 --std=c++11
ALL_CPPFLAGS=-I./include/ -Wall
ifneq "$(NO_THREADS)" "true"
    CC += -pthread
    CXX += -pthread
else
    ALL_CPPFLAGS += -DNO_THREADS
endif
LDLIBS=-lgmp -lgmpxx
ALL_CPPFLAGS += $(CPPFLAGS)
OBJDUMP_FLAGS=-dCSr

empty:=
space:= $(empty) $(empty)

libsources = atkin.cc util.cc set0.cc set1.cc set2.cc set3.cc set4.cc set5.cc \
	     set6.cc
sources = $(libsources) main.cc

hlibs = $(subst $(space),|,$(basename $(libsources)))

vpath %.cc src/
vpath %.hh include/

default:

libs: $(subst .cc,.o,$(libsources))

clean:
	rm -f *.?ps
	rm -f *.o
	rm -rf bin

cleanall: clean
	rm -f *.d
	rm -f *.d.????

%.d: %.cc Makefile
	$(CC) -MM $(ALL_CPPFLAGS) $< > $@

include $(if $(filter-out clean cleanall,$(MAKECMDGOALS)),$(sources:%.cc=%.d))
#doesn't include *.d if MAKECMDGOALS consists *only* of clean and cleanall

%.o: %.cc
	$(CXX) -o $@ $< -c $(ALL_CPPFLAGS) $(CXXFLAGS) 2>&1

main: $(subst .cc,.o,$(sources))
	mkdir -p bin
	$(CXX) -o bin/$@ $^ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) 2>&1

%.cps: %.cc
	$(CXX) -S $(ALL_CPPFLAGS) $(CXXFLAGS) $< -o $@

%.dps: %.o
	objdump $(OBJDUMP_FLAGS) $< > $@

%.pps: %.cc
	$(CC) -E $(ALL_CPPFLAGS) $< -o $@

%.hps: %.hh
	$(CC) -E $(ALL_CPPFLAGS) $< -o $@
