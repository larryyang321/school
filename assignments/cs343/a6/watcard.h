#ifndef WATCARD_H
#define WATCARD_H

#include <uFuture.h>

class WATCard {
    WATCard(const WATCard &);
    WATCard &operator=(const WATCard &);

public:
    typedef Future_ISM<WATCard *> FWATCard;

    WATCard();

    void deposit(unsigned int amount);
    void withdraw(unsigned int amount);

    unsigned int getBalance();
};

#endif
