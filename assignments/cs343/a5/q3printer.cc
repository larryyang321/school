#include <iostream>

#include "q3printer.h"
#include "q3tallyVotes.h"
#include "q3voter.h"

std::ostream& operator<< (std::ostream &o, const PrinterInfo& info) {
    o << (char)info.state;

    switch (info.state) {
        case Voter::States::Start:
        case Voter::States::Complete:
        case Voter::States::Barging:
            break;
        case Voter::States::Vote:
        case Voter::States::Finished:
            o << " " << (info.vote == 0 ? "p" : "s");
            break;
        case Voter::States::Block:
        case Voter::States::Unblock:
            o << " " << info.numBlocked;
            break;
        // TODO: default exception
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

    if (state == Voter::States::Finished) {
        for (unsigned int i = 0; i < voters; ++i) {
            if (i == id) {
                std::cout << *info[i] << "\t";

                delete info[i];
                info[i] = NULL;
            } else {
                std::cout << "...\t";
            }
        }
        std::cout << std::endl;
    }
}

void Printer::print(unsigned int id, Voter::States state,
                    unsigned int numBlocked) {
    if (info[id]) {
        flush();
    }

    info[id] = new PrinterInfo(state);
    info[id]->numBlocked = numBlocked;
}
