#include "q3printer.h"
#include "q3tallyVotes.h"
#include "q3voter.h"

#if defined(IMPLTYPE_EXT)
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : waiters(0), group(group), printer(printer), countPicture(0),
          countStatue(0), result(0) { }

TallyVotes::Tour TallyVotes::vote(unsigned int id, Tour ballot) {
    printer.print(id, Voter::States::Vote, ballot);
    if (ballot == TallyVotes::Tour::Picture) {
        ++countPicture;
    } else {
        ++countStatue;
    }

    ++waiters;
    if (waiters < group) {
        printer.print(id, Voter::States::Block, waiters);
        _Accept(vote);
        printer.print(id, Voter::States::Unblock, waiters - 1);
    } else {
        printer.print(id, Voter::States::Complete);

        result = (countPicture > countStatue) ? 0 : 1;
        countPicture = countStatue = 0;
    }
    --waiters;

    printer.print(id, Voter::States::Finished, TallyVotes::Tour(result));
    return TallyVotes::Tour(result);
}

#elif defined(IMPLTYPE_INT)
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : waiters(0), group(group), printer(printer), countPicture(0),
          countStatue(0), result(0) { }

TallyVotes::Tour TallyVotes::vote(unsigned int id, Tour ballot) {
    printer.print(id, Voter::States::Vote, ballot);
    if (ballot == TallyVotes::Tour::Picture) {
        ++countPicture;
    } else {
        ++countStatue;
    }

    ++waiters;
    if (waiters < group) {
        printer.print(id, Voter::States::Block, waiters);
        blocked.wait();
        printer.print(id, Voter::States::Unblock, waiters - 1);
    } else {
        printer.print(id, Voter::States::Complete);
        result = (countPicture > countStatue) ? 0 : 1;
        countPicture = countStatue = 0;

    }
    --waiters;

    blocked.signal();

    printer.print(id, Voter::States::Finished, TallyVotes::Tour(result));
    return TallyVotes::Tour(result);
}

#elif defined(IMPLTYPE_INTB)
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : tickets(0), serving(group), waiters(0), group(group),
          printer(printer), countPicture(0), countStatue(0), result(0) { }

void TallyVotes::wait() {
    blocked.wait();

    while (rand() % 5 == 0) {
        _Accept(vote) {
        } _Else {
        }
    }
}

void TallyVotes::signalAll() {
    while (!blocked.empty()) {
        blocked.signal();
    }
}

TallyVotes::Tour TallyVotes::vote(unsigned int id, Tour ballot) {
    unsigned int ticket = tickets++;
    while (ticket >= serving) {
        printer.print(id, Voter::States::Barging);
        wait();
    }

    printer.print(id, Voter::States::Vote, ballot);
    if (ballot == TallyVotes::Tour::Picture) {
        ++countPicture;
    } else {
        ++countStatue;
    }

    ++waiters;
    if (waiters < group) {
        printer.print(id, Voter::States::Block, waiters);
        wait();
        printer.print(id, Voter::States::Unblock, waiters - 1);
    } else {
        printer.print(id, Voter::States::Complete);

        result = (countPicture > countStatue) ? 0 : 1;
        countPicture = countStatue = 0;

        signalAll();
    }
    --waiters;

    if (!waiters) {
        serving += group;
        signalAll();
    }

    printer.print(id, Voter::States::Finished, TallyVotes::Tour(result));
    return TallyVotes::Tour(result);
}

#elif defined(IMPLTYPE_AUTO)
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : doneVoting(false), waiters(0), group(group), printer(printer),
          countPicture(0), countStatue(0), result(0) { }

TallyVotes::Tour TallyVotes::vote(unsigned int id, Tour ballot) {
    printer.print(id, Voter::States::Vote, ballot);
    if (ballot == TallyVotes::Tour::Picture) {
        ++countPicture;
    } else {
        ++countStatue;
    }

    if (!waiters) {
        doneVoting = false;
    }

    if (waiters < group - 1) {
        WAITUNTIL(doneVoting, printer.print(id, Voter::States::Block, ++waiters),
                  printer.print(id, Voter::States::Unblock, --waiters));
    } else {
        printer.print(id, Voter::States::Complete);

        result = (countPicture > countStatue) ? 0 : 1;
        countPicture = countStatue = 0;

        doneVoting = true;
    }

    printer.print(id, Voter::States::Finished, TallyVotes::Tour(result));
    RETURN(TallyVotes::Tour(result));
}

#elif defined(IMPLTYPE_TASK)
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : waiters(0), group(group), printer(printer), countPicture(0),
          countStatue(0), result(0), id(0), ballot(TallyVotes::Tour::NONE) { }

TallyVotes::Tour TallyVotes::vote(unsigned int id, Tour ballot) {
    TallyVotes::id = id;
    TallyVotes::ballot = ballot;

    blocked.wait(id);

    return ballot;
}

void TallyVotes::main() {
    for (;;) {
        _Accept(~TallyVotes) {
            return;
        } or _Accept(vote) {
            printer.print(id, Voter::States::Vote, ballot);
            if (ballot == TallyVotes::Tour::Picture) {
                ++countPicture;
            } else {
                ++countStatue;
            }

            printer.print(id, Voter::States::Block, ++waiters);
            if (waiters == group) {
                printer.print(id, Voter::States::Complete);

                result = (countPicture > countStatue) ? 0 : 1;
                countPicture = countStatue = 0;

                while (!blocked.empty()) {
                    printer.print(blocked.front(), Voter::States::Unblock,
                                  --waiters);
                    printer.print(blocked.front(), Voter::States::Finished,
                                  TallyVotes::Tour(result));

                    blocked.signalBlock();
                }
            }
        }
    }
}

#else
    #error unsupported voter type
#endif
