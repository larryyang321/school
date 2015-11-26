#ifndef STUDENT_H
#define STUDENT_H

#include "groupoff.h"
#include "nameserver.h"
#include "printer.h"
#include "student.h"
#include "watcardoffice.h"

_Task Student {
    void main();

public:
    Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice,
            Groupoff &groupoff, unsigned int id, unsigned int maxPurchases);
};

#endif
