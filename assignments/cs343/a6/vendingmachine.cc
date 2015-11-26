#include "nameserver.h"
#include "printer.h"
#include "vendingmachine.h"
#include "watcard.h"

void VendingMachine::main() {
    // TODO
}

VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer,
                               unsigned int id, unsigned int sodaCost,
                               unsigned int maxStockPerFlavour) {
    // TODO
}

void VendingMachine::buy(Flavours flavour, WATCard &card) {
    // TODO
}

unsigned int *VendingMachine::inventory() {
    // TODO
    return (unsigned int *)0;
}

void VendingMachine::restocked() {
    // TODO
}

_Nomutex unsigned int VendingMachine::cost() {
    // TODO
    return 0;
}

_Nomutex unsigned int VendingMachine::getId() {
    // TODO
    return 0;
}
