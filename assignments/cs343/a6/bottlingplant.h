#ifndef BOTTLINGPLANT_H
#define BOTTLINGPLANT_H

#include "nameserver.h"
#include "printer.h"

_Task BottlingPlant {
    void main();

public:
    _Event Shutdown {};

    BottlingPlant(Printer &prt, NameServer &nameServer,
                  unsigned int numVendingMachines,
                  unsigned int maxShippedPerFlavour,
                  unsigned int maxStockPerFlavour,
                  unsigned int timeBetweenShipments);

    void getShipment(unsigned int cargo[]);
};

#endif
