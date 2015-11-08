#ifndef PRINTER_H
#define PRINTER_H

#include "q2voter.h"

struct PrinterInfo {
    Voter::States state;
    TallyVotes::Tour vote;
    unsigned int numBlocked;

    PrinterInfo(Voter::States state)
            : state(state), vote(TallyVotes::Tour::NONE), numBlocked(-1) { }
};

_Monitor Printer {
    unsigned int voters;

    PrinterInfo **info;

    void flush();

public:
    Printer(unsigned int voters);
    ~Printer();

    void print(unsigned int id, Voter::States state);
    void print(unsigned int id, Voter::States state, TallyVotes::Tour vote);
    void print(unsigned int id, Voter::States state, unsigned int numBlocked);
};

#endif
