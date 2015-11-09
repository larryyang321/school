#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"

#if defined(IMPLTYPE_MC)

#elif defined(IMPLTYPE_BAR)
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : uBarrier(group), group(group), printer(printer), countPicture(0),
          countStatue(0) { }

TallyVotes::~TallyVotes() { }

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

#elif defined(IMPLTYPE_SEM)
TallyVotes::TallyVotes(unsigned int group, Printer &printer)
        : count(0), group(group), printer(printer), countPicture(0),
          countStatue(0) {
    task_lock = new uSemaphore*[group];
    for (unsigned int i = 0; i < group; ++i) {
        task_lock[i] = new uSemaphore(0);
        taskIds = new unsigned int[group];
    }
    countPicture = countStatue = 0;
}

TallyVotes::~TallyVotes() {
    for (unsigned int i = 0; i < group; ++i) {
        delete task_lock[i];
    }

    delete[] task_lock;
    delete[] taskIds;
}

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Tour ballot) {
    critical_lock.P();

    printer.print(id, Voter::States::Vote, ballot);
    if (ballot == TallyVotes::Tour::Picture) {
        ++countPicture;
    } else {
        ++countStatue;
    }

    if (count == group - 1) {
        result = (countPicture > countStatue) ? 0 : 1;

        printer.print(id, Voter::States::Complete);
        printer.print(id, Voter::States::Finished, result);

        for (unsigned int i = 0; i < group - 1; ++i) {
            printer.print(taskIds[i], Voter::States::Unblock, group - i - 2);
            printer.print(taskIds[i], Voter::States::Finished, result);

            task_lock[i]->V();
        }

        count = 0;
        countPicture = countStatue = 0;
        critical_lock.V();
    } else {
        printer.print(id, Voter::States::Block, count + 1);
        taskIds[count] = id;
        task_lock[count++]->P(critical_lock);
    }

    return TallyVotes::Tour(result);
}

#else
    #error unsupported voter type
#endif
