#ifndef TALLYVOTES_H
#define TALLYVOTES_H

#include "MPRNG.h"

_Monitor Printer;

#if defined(IMPLTYPE_EXT)
_Monitor TallyVotes {

#elif defined(IMPLTYPE_INT)
_Monitor TallyVotes {
    uCondition blocked;

#elif defined(IMPLTYPE_INTB)
_Monitor TallyVotes {
    unsigned int tickets;
    unsigned int serving;

    uCondition blocked;

    void wait();
    void signalAll();

#elif defined(IMPLTYPE_AUTO)
#include "AutomaticSignal.h"
_Monitor TallyVotes {
    AUTOMATIC_SIGNAL;

    bool doneVoting;

#elif defined(IMPLTYPE_TASK)
_Task TallyVotes {
    uCondition blocked;

    void main();

#else
    #error unsupported voter type
#endif

    unsigned int waiters;

    unsigned int group;
    Printer &printer;

    unsigned int countPicture;
    unsigned int countStatue;

    unsigned int result;

public:
    TallyVotes(unsigned int group, Printer &printer);
    enum Tour { Picture, Statue, NONE };
    Tour vote(unsigned int id, Tour ballot);

#if defined(IMPLTYPE_TASK)
private:
    unsigned int id;
    Tour ballot;
#endif
};

#endif
