#ifndef VOTER_H
#define VOTER_H

#include "q2tallyVotes.h"

_Task Voter {
    unsigned int id;
    TallyVotes &voteTallier;
    Printer &printer;

    void main();

public:
    enum States { Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U',
                  Complete = 'C', Finished = 'F' };
    Voter(unsigned int id, TallyVotes &voteTallier, Printer &printer);
};

#endif
