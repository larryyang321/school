#include <iostream>

#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"

std::ostream& operator<< (std::ostream &o, const PrinterInfo& info) {
    o << (char)info.state;

    switch(info.state) {
        case Voter::States::Start:
        case Voter::States::Complete:
            break;
        case Voter::States::Vote:
            o << " " << (info.vote == 0 ? "p" : "s");
            break;
        case Voter::States::Block:
        case Voter::States::Unblock:
        case Voter::States::Finished:
            o << " " << info.numBlocked;
            break;
    }

    return o;
}

Printer::Printer(unsigned int voters) : voters(voters) {
    info = new PrinterInfo*[voters];
    for (unsigned int i = 0; i < voters; ++i) {
        info[i] = NULL;

        std::cout << "Voter" << i << "\t";
    }
    std::cout << std::endl;

    for (unsigned int i = 0; i < voters; ++i) {
        std::cout << "=======\t";
    }
    std::cout << std::endl;
}

Printer::~Printer() {
    flush();
    std::cout << "=================" << std::endl;
    std::cout << "All tours started" << std::endl;

    delete[] info;
}

void Printer::flush() {
    for (unsigned int i = 0; i < voters; ++i) {
        if (info[i]) {
            std::cout << *info[i];

            delete info[i];
            info[i] = NULL;
        }
        std::cout << "\t";
    }
    std::cout << std::endl;
}

void Printer::print(unsigned int id, Voter::States state) {
    if (info[id]) {
        flush();
    }

    info[id] = new PrinterInfo(state);
}

void Printer::print(unsigned int id, Voter::States state,
                    TallyVotes::Tour vote) {
    if (info[id]) {
        flush();
    }

    info[id] = new PrinterInfo(state);
    info[id]->vote = vote;
}

void Printer::print(unsigned int id, Voter::States state,
                    unsigned int numBlocked) {
    if (info[id]) {
        flush();
    }

    info[id] = new PrinterInfo(state);
    info[id]->numBlocked = numBlocked;
}
