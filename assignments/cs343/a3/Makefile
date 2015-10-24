TYPE := int
BTYPE := int
SENTINEL := -1
KIND := NOBUSY
OPT := -O2

UXX=`which u++`
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD ${OPT} -DTYPE="${TYPE}" -DBTYPE="${BTYPE}" -DSENTINEL="${SENTINEL}" -D"${KIND}"


all: q1 q2
q1: mergesort
q2: buffer


.SUFFIXES:
.SUFFIXES: .cc .o

.cc.o:
	$(UXX) $(CXXFLAGS) -c $< -o $@


-include ImplType

q1mergesort.o: q1mergesort.cc q1mergesort.h
ifeq (${IMPLTYPE},${TYPE})
mergesort: q1mergesort.o
	$(UXX) $(CXXFLAGS) $^ -o $@
else
ifeq (${TYPE},)
TYPE=${IMPLTYPE}
mergesort: q1mergesort.o
	$(UXX) $(CXXFLAGS) $^ -o $@
else
.PHONY: mergesort
mergesort:
	rm -f ImplType
	touch q1mergesort.h
	sleep 1
	${MAKE} TYPE="${TYPE}" SENTINEL="${SENTINEL}"
endif
endif

ImplType:
	echo "IMPLTYPE=${TYPE}" > ImplType


-include ImplKind

q2buffer.o: q2buffer.cc q2buffer.h MPRNG.h
ifeq (${IMPLKIND},${KIND})
buffer: q2buffer.o
	$(UXX) $(CXXFLAGS) $^ -o $@
else
ifeq (${KIND},)
KIND=${IMPLKIND}
buffer: q2buffer.o
	$(UXX) $(CXXFLAGS) $^ -o $@
else
.PHONY: buffer
buffer:
	rm -f ImplKind
	touch q2buffer.h
	sleep 1
	${MAKE} KIND="${KIND}"
endif
endif

ImplKind:
	echo "IMPLKIND=${KIND}" > ImplKind


clean:
	rm -f *.d
	rm -f *.o
	rm -f ImplKind
	rm -f ImplType
	rm -f buffer
	rm -f mergesort
