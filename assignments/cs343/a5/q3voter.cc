#include "q3tallyVotes.h"
#include "q3printer.h"
#include "q3voter.h"

extern MPRNG mprng;

Voter::Voter(unsigned int id, TallyVotes &voteTallier, Printer &printer)
        : id(id), voteTallier(voteTallier), printer(printer) { }

void Voter::main() {
    yield(mprng(19));

    printer.print(id, Voter::States::Start);

    TallyVotes::Tour ballot = TallyVotes::Tour(mprng(1));
    voteTallier.vote(id, ballot);
}
