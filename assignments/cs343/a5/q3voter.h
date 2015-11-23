#ifndef VOTER_H
#define VOTER_H

#include "q3tallyVotes.h"

_Task Voter {
    unsigned int id;
    TallyVotes &voteTallier;
    Printer &printer;

    void main();

public:
    enum States { Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U',
                  Barging = 'b', Complete = 'C', Finished = 'F' };
    Voter(unsigned int id, TallyVotes &voteTallier, Printer &printer);
};

#endif
