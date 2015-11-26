#ifndef VENDINGMACHINE_H
#define VENDINGMACHINE_H

// #include "nameserver.h"
#include "printer.h"
#include "watcard.h"

_Task NameServer;

_Task VendingMachine {
    void main();

public:
    enum Flavours {
        // TODO
    };

    _Event Funds {};
    _Event Stock {};

    VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id,
                   unsigned int sodaCost, unsigned int maxStockPerFlavour);

    void buy(Flavours flavour, WATCard &card);

    unsigned int *inventory();

    void restocked();

    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif
