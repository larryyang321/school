KIND := HEAP
TYPE := SEM

UXX = `which u++`
CXXFLAGS = -g -multi -Wall -Wno-unused-label -O2 -DIMPLTYPE_"${TYPE}"

q2OBJECTS = q2main.o q2printer.o q2tallyVotes.o q2voter.o


all: new vote


.SUFFIXES:
.SUFFIXES: .cc .o

.cc.o:
	$(UXX) $(CXXFLAGS) -c $< -o $@


new: q1new.o
	$(UXX) -multi -O2 -nodebug -D"${KIND}" $^ -o $@


-include ImplType

ifeq (${IMPLTYPE},${TYPE})
vote: $(q2OBJECTS)
	$(UXX) $(CXXFLAGS) $^ -o $@
else
ifeq (${TYPE},)
TYPE=${IMPLTYPE}
vote: $(q2OBJECTS)
	$(UXX) $(CXXFLAGS) $^ -o $@
else
.PHONY: vote
vote:
	rm -f ImplType
	touch q2tallyVotes.h
	sleep 1
	${MAKE} vote TYPE="${TYPE}"
endif
endif

ImplType:
	echo "IMPLTYPE=${TYPE}" > ImplType

q2main.o: q2main.cc q2tallyVotes.h q2printer.h q2voter.h
q2printer.o: q2printer.cc q2tallyVotes.h q2printer.h q2voter.h
q2tallyVotes.o: q2tallyVotes.cc q2tallyVotes.h q2printer.h q2voter.h
q2voter.o: q2voter.cc q2tallyVotes.h q2printer.h q2voter.h


clean:
	rm -f *.d
	rm -f *.o
	rm -f ImplType
	rm -f new
	rm -f vote
