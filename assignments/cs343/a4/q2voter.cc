#include "q2tallyVotes.h"
#include "q2printer.h"
#include "q2voter.h"

extern MPRNG mprng;

Voter::Voter(unsigned int id, TallyVotes &voteTallier, Printer &printer)
        : id(id), voteTallier(voteTallier), printer(printer) { }

void Voter::main() {
    printer.print(id, Voter::Start);

    yield(mprng(19));

    TallyVotes::Tour ballot = TallyVotes::Tour(mprng(1));
    voteTallier.vote(id, ballot);
}
