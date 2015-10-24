TYPE := int
SENTINEL := -1

UXX=`which u++`
CXXFLAGS = -g -Wall -Wno-unused-label -MMD -DTYPE="${TYPE}" -DSENTINEL="${SENTINEL}"


all: q1 q2 q3
q1: binsertsort
q2: cardgame
q3: q3multi q3uni


.SUFFIXES:
.SUFFIXES: .cc .o

.cc.o:
	$(UXX) $(CXXFLAGS) -c $< -o $@


-include ImplType

q1binsertsort.o: q1binsertsort.cc q1binsertsort.h
ifeq (${IMPLTYPE},${TYPE})
binsertsort: q1binsertsort.o
	$(UXX) $(CXXFLAGS) $^ -o $@
else
ifeq (${TYPE},)
TYPE=${IMPLTYPE}
binsertsort: q1binsertsort.o
	$(UXX) $(CXXFLAGS) $^ -o $@
else
.PHONY: binsertsort
binsertsort:
	rm -f ImplType
	touch q1binsertsort.h
	sleep 1
	${MAKE} TYPE="${TYPE}" SENTINEL="${SENTINEL}"
endif
endif

ImplType:
	echo "IMPLTYPE=${TYPE}" > ImplType

q2cardgame.o: q2cardgame.cc q2cardgame.h PRNG.h
cardgame: q2cardgame.o
	$(UXX) $(CXXFLAGS) $^ -o $@

q3multi: q3parallel.cc
	$(UXX) -multi $^ -o $@

q3uni: q3parallel.cc
	$(UXX) $^ -o $@


clean:
	rm -f *.d
	rm -f *.o
	rm -f ImplType
	rm -f binsertsort
	rm -f cardgame
	rm -f q3uni
	rm -f q3multi
