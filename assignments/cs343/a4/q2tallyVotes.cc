#include "q2printer.h"
#include "q2tallyVotes.h"

TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : group(group), printer(printer) { }

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Tour ballot) {
    return ballot;
}
