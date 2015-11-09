#ifndef TALLYVOTES_H
#define TALLYVOTES_H

#include "MPRNG.h"

_Monitor Printer;

#if defined(IMPLTYPE_MC)
class TallyVotes {

#elif defined(IMPLTYPE_BAR)
#include <uBarrier.h>
_Cormonitor TallyVotes : public uBarrier {
    void last();

#elif defined(IMPLTYPE_SEM)
#include <uSemaphore.h>
class TallyVotes {
    uSemaphore critical_lock;
    uSemaphore **task_lock;

    unsigned int count;
    unsigned int *taskIds;

#else
    #error unsupported voter type
#endif

    unsigned int group;
    Printer &printer;

    unsigned int countPicture;
    unsigned int countStatue;

    unsigned int result;

public:
    TallyVotes(unsigned int group, Printer &printer);
    ~TallyVotes();
    enum Tour { Picture, Statue, NONE };
    Tour vote(unsigned int id, Tour ballot);
};

#endif
