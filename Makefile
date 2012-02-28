CC=/usr/bin/gcc-4.6.2 --std=c++0x -pthread
CXX=/usr/bin/g++-4.6.2 --std=c++0x -pthread
LDLIBS=
ALL_CPPFLAGS=-I./include/ -Wall $(CPPFLAGS)
OBJDUMP_FLAGS=-dCSr

empty:=
space:= $(empty) $(empty)

libsources = atkin.cc util.cc
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
	@set -e; \
	rm -f $@; \
	echo -n "$(CC) -MM $<; et cetera ... "; \
	$(CC) -MM $(ALL_CPPFLAGS) $< > $@.$$$$; \
	sed -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	echo -n "$* : " >> $@ ; \
	awk 'BEGIN { RS = " " } ; /include\/($(hlibs)).hh/ { print $$0 }' $@ | sed -n -e 's,^[^/]*/\([^.]*\).hh,\1.o ,w/dev/stdout' | tr -d '\012' >> $@; \
	echo >> $@; \
	rm -f $@.$$$$; \
	echo $@;

include $(if $(filter-out clean cleanall,$(MAKECMDGOALS)),$(sources:%.cc=%.d))
#doesn't include *.d if MAKECMDGOALS consists *only* of clean and cleanall

%.o: %.cc
	$(CXX) -o $@ $< -c $(ALL_CPPFLAGS) $(CXXFLAGS) 2>&1

%: %.o
	mkdir -p bin
	$(CXX) -o bin/$@ $^ $(LDFLAGS) $(LDLIBS) 2>&1

%.cps: %.cc
	$(CXX) -S $(ALL_CPPFLAGS) $(CXXFLAGS) $< -o $@

%.dps: %.o
	objdump $(OBJDUMP_FLAGS) $< > $@

%.pps: %.cc
	$(CC) -E $(ALL_CPPFLAGS) $< -o $@

%.hps: %.hh
	$(CC) -E $(ALL_CPPFLAGS) $< -o $@
