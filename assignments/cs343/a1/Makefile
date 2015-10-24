CXX=$(shell test -f /usr/bin/g++-4.9 && echo /usr/bin/g++-4.9 || echo `which g++`)
UXX=`which u++`
CXXFLAGS = -g -Wall -Wno-unused-label -MMD -std=c++11


all: q1 q2 q3
q1: longjmp throwcatch
q2: fixup resumption
q3: utf8 utf8gen utf8input


.SUFFIXES:
.SUFFIXES: .cc .o .uo

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
.cc.uo:
	$(UXX) $(CXXFLAGS) -c $< -o $@


fixup: q2fixup.o
	$(CXX) $(CXXFLAGS) $^ -o $@

longjmp: q1longjmp.o
	$(CXX) $(CXXFLAGS) $^ -o $@

resumption: q2resumption.uo
	$(UXX) $(CXXFLAGS) $^ -o $@

throwcatch: q1throwcatch.o
	$(CXX) $(CXXFLAGS) $^ -o $@

utf8: q3utf8.uo
	$(UXX) $(CXXFLAGS) $^ -o $@

utf8gen: q3gen.o
	$(CXX) $(CXXFLAGS) $^ -o $@

utf8input: utf8gen
	./utf8gen > utf8input


clean:
	rm -f *.d
	rm -f *.o
	rm -f *.uo
	rm -f fixup
	rm -f longjmp
	rm -f resumption
	rm -f throwcatch
	rm -f utf8
	rm -f utf8gen
	rm -f utf8input
