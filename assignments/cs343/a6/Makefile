UXX = `which u++`

CXXFLAGS += -MMD
CXXFLAGS += -g -Wall -Wno-unused-label

OBJECTS = $(addsuffix .o, bank bottlingplant config groupoff main nameserver parent printer student truck vendingmachine watcard watcardoffice)

DEPENDS = ${OBJECTS:.o=.d}


all: soda


.SUFFIXES:
.SUFFIXES: .cc .o

.cc.o:
	$(UXX) $(CXXFLAGS) -c $< -o $@


soda: $(OBJECTS)
	$(UXX) $(CXXFLAGS) $^ -o $@


-include ${DEPENDS}


clean:
	rm -f *.d
	rm -f *.o
	rm -f soda
