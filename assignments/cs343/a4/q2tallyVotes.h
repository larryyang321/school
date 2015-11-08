#ifndef TALLYVOTES_H
#define TALLYVOTES_H

#include "MPRNG.h"

_Monitor Printer;

#if defined(IMPLTYPE_MC)
class TallyVotes {

#elif defined(IMPLTYPE_BAR)
#include <uBarrier.h>
_Cormonitor TallyVotes : public uBarrier {
    unsigned int countPicture;
    unsigned int countStatue;

    void last();

#elif defined(IMPLTYPE_SEM)
class TallyVotes {

#else
    #error unsupported voter type
#endif

    unsigned int group;
    Printer &printer;

    unsigned int result;

public:
    TallyVotes(unsigned int group, Printer &printer);
    enum Tour { Picture, Statue, NONE };
    Tour vote(unsigned int id, Tour ballot);
};

#endif
