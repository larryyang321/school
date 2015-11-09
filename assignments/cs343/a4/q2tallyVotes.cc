#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"

#if defined(IMPLTYPE_MC)
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : barging(false), waiters(0), group(group), printer(printer),
          countPicture(0), countStatue(0), result(0) { }

TallyVotes::~TallyVotes() { }

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Tour ballot) {
    exclusion.acquire();

    // prime barge lock
    if (barging) {
        bargeLock.wait(exclusion);
    }

    printer.print(id, Voter::States::Vote, ballot);
    if (ballot == TallyVotes::Tour::Picture) {
        ++countPicture;
    } else {
        ++countStatue;
    }

    if (waiters == group - 1) {
        printer.print(id, Voter::States::Complete);
    } else {
        // barge
        if (bargeLock.empty()) {
            barging = false;
        } else {
            bargeLock.signal();
        }

        printer.print(id, Voter::States::Block, ++waiters);
        blockedLock.wait(exclusion);
        printer.print(id, Voter::States::Unblock, --waiters);

        // clear barge flag when no tasks are blocks
        if (blockedLock.empty()) {
            barging = false;
        }
    }

    result = (countPicture > countStatue) ? 0 : 1;
    countPicture = countStatue = 0;

    if (blockedLock.empty()) {
        // barge
        if (!bargeLock.empty()) {
            bargeLock.signal();
        }
    } else {
        // unblock waitings tasks, enable barging
        barging = true;
        blockedLock.signal();
    }

    exclusion.release();

    printer.print(id, Voter::States::Finished, TallyVotes::Tour(result));
    return TallyVotes::Tour(result);
}

#elif defined(IMPLTYPE_BAR)
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : uBarrier(group), group(group), printer(printer), countPicture(0),
          countStatue(0), result(0) { }

TallyVotes::~TallyVotes() { }

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Tour ballot) {
    printer.print(id, Voter::States::Vote, ballot);
    if (ballot == TallyVotes::Tour::Picture) {
        ++countPicture;
    } else {
        ++countStatue;
    }

    if (waiters() == total() - 1) {
        // complete the barrier (last waiter)
        uBarrier::block();
        printer.print(id, Voter::States::Complete);
    } else {
        printer.print(id, Voter::States::Block, waiters() + 1);
        uBarrier::block();
        printer.print(id, Voter::States::Unblock, waiters());
    }

    printer.print(id, Voter::States::Finished, TallyVotes::Tour(result));
    return TallyVotes::Tour(result);
}

void TallyVotes::last() {
    result = (countPicture > countStatue) ? 0 : 1;
    countPicture = countStatue = 0;
}

#elif defined(IMPLTYPE_SEM)
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : waiters(0), group(group), printer(printer), countPicture(0),
          countStatue(0), result(0) {
    // initialize array of waiting task_ids
    waiterIds = new unsigned int[group];

    // initialize array of "locks" for each waiting task
    taskLocks = new uSemaphore*[group];
    for (unsigned int i = 0; i < group; ++i) {
        taskLocks[i] = new uSemaphore(0);
    }
}

TallyVotes::~TallyVotes() {
    delete[] waiterIds;

    for (unsigned int i = 0; i < group; ++i) {
        delete taskLocks[i];
    }
    delete[] taskLocks;
}

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Tour ballot) {
    exclusion.P();

    printer.print(id, Voter::States::Vote, ballot);
    if (ballot == TallyVotes::Tour::Picture) {
        ++countPicture;
    } else {
        ++countStatue;
    }

    if (waiters == group - 1) {
        // final task: determine result, print finished, wake waiting tasks
        result = (countPicture > countStatue) ? 0 : 1;
        countPicture = countStatue = 0;

        printer.print(id, Voter::States::Complete);
        printer.print(id, Voter::States::Finished, TallyVotes::Tour(result));

        // finish all waiting tasks
        for (unsigned int i = 0; i < group - 1; ++i) {
            printer.print(waiterIds[i], Voter::States::Unblock, group - i - 2);
            printer.print(waiterIds[i], Voter::States::Finished,
                          TallyVotes::Tour(result));

            taskLocks[i]->V();
        }

        waiters = 0;
        exclusion.V();
    } else {
        printer.print(id, Voter::States::Block, waiters + 1);
        // sleep until all tasks are done
        waiterIds[waiters] = id;
        taskLocks[waiters++]->P(exclusion);
    }

    return TallyVotes::Tour(result);
}

#else
    #error unsupported voter type
#endif
