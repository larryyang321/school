#ifndef WATCARDOFFICE_H
#define WATCARDOFFICE_H

#include "bank.h"
#include "printer.h"
#include "watcard.h"

struct Args {
    // TODO
};

_Task WATCardOffice {
    struct Job {
        Args args;
        WATCard::FWATCard result;
        Job(Args args) : args(args) {}
    };

    _Task Courier {
        // TODO
    };

    void main();

public:
    _Event Lost {};

    WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers);

    WATCard::FWATCard create(unsigned int sid, unsigned int amount);
    WATCard::FWATCard transfer(unsigned int sid, unsigned int amount,
                               WATCard *card);

    Job *requestWork();
};

#endif
