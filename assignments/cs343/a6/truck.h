#ifndef TRUCK_H
#define TRUCK_H

#include "bottlingplant.h"
#include "nameserver.h"
#include "printer.h"

_Task Truck {
    void main();

public:
    Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
          unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};

#endif
