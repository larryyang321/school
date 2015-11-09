#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"

#ifdef IMPLTYPE_BAR
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : uBarrier(group), countPicture(0), countStatue(0), group(group),
          printer(printer) {
}

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Tour ballot) {
    printer.print(id, Voter::States::Vote, ballot);
    if (ballot == TallyVotes::Tour::Picture) {
        ++countPicture;
    } else {
        ++countStatue;
    }

    if (waiters() == total() - 1) {
        uBarrier::block();
        printer.print(id, Voter::States::Complete);
    } else {
        printer.print(id, Voter::States::Block, waiters() + 1);
        uBarrier::block();
        printer.print(id, Voter::States::Unblock, waiters());
    }

    printer.print(id, Voter::States::Finished, result);
    return TallyVotes::Tour(result);
}

void TallyVotes::last() {
    result = (countPicture > countStatue) ? 0 : 1;
    countPicture = countStatue = 0;
}
#endif
