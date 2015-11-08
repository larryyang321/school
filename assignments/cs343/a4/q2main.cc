#include <iostream>

#include "MPRNG.h"

#include "q2printer.h"
#include "q2tallyVotes.h"
#include "q2voter.h"

MPRNG mprng;

void uMain::main() {
    int V = 6, G = 3, seed = getpid();

    switch (argc) {
        case 4:
            seed = std::stoi(argv[3]);
            if (seed <= 0) {
                std::cerr << "Error: seed must be positive." << std::endl;
                exit(-1);
            }
        case 3:
            G = std::stoi(argv[2]);
            if (G <= 0 || !(G & 0x1)) {
                std::cerr << "Error: G must be positive and odd." << std::endl;
                exit(-1);
            }
        case 2:
            V = std::stoi(argv[1]);
            if (V <= 0 || V % G) {
                std::cerr << "Error: V must be postive and a multiple of G.";
                std::cerr << std::endl;
                exit(-1);
            }
        case 1:
            break;
        default:
            std::cerr << "Usage: ./vote [V [G [seed]]" << std::endl;
            exit(-1);
    }

    mprng.seed(seed);

    Printer printer(V);
    TallyVotes tallyVotes(V, printer);

    Voter *voter[V];
    for (int i = 0; i < V; ++i) {
        voter[i] = new Voter(i, tallyVotes, printer);
    }

    for (int i = 0; i < V; ++i) {
        delete voter[i];
    }
}
