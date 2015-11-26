#include "bank.h"
#include "printer.h"
#include "watcard.h"
#include "watcardoffice.h"

void WATCardOffice::main() {
    // TODO
}

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank,
                             unsigned int numCouriers) {
    // TODO
}

WATCard::FWATCard WATCardOffice::create(unsigned int sid,
                                        unsigned int amount) {
    // TODO
    return WATCard::FWATCard();
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid,
                                          unsigned int amount, WATCard *card) {
    // TODO
    return WATCard::FWATCard();
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    // TODO
    return (WATCardOffice::Job *)0;
}
