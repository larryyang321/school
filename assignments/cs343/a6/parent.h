#ifndef PARENT_H
#define PARENT_H

#include "bank.h"
#include "printer.h"

_Task Parent {
    void main();

public:
    Parent(Printer &prt, Bank &bank, unsigned int numStudents,
           unsigned int parentalDelay);
};

#endif
