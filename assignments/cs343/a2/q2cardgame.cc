#include <cmath>
#include <iostream>

#include "PRNG.h"
#include "q2cardgame.h"

PRNG *prng;

std::ostream& operator<< (std::ostream &o, const PlayerInfo& info) {
    if (info.last) {
        o << "#" << info.deck << "#";
    } else {
        o << info.took << ":" << info.deck;

        if (info.deck) {
            o << (info.pass_right ? ">" : "<");
        } else {
            o << "#";
        }
    }

    if (info.dead) {
        o << "X";
    }

    return o;
}

Player::Player(Printer &printer, unsigned int id)
        : printer(printer), id(id), deck(0), lp(NULL), rp(NULL) { }

void Player::main() {
    suspend();

    for (;;) {
        // Game ends when last player receives the deck
        if (numPlayers == 1) {
            printer.prt(id, 0, numPlayers);
            return;
        }

        // Player should not take more than the number of cards in the deck
        unsigned int numTaken = fmin(deck, (*prng)(1, 8));
        printer.prt(id, numTaken, numPlayers);
        if (deck % DEATH_DECK_DIVISOR == 0) {
            // Remove self, modify players to "skip over" self in the future
            --numPlayers;

            lp->rp = rp;
            rp->lp = lp;
        }

        unsigned int deckAfter = deck - numTaken;
        if (deckAfter == 0) {
            // Game ends
            return;
        }

        // Players pass to right if number of cards is even
        if (deckAfter % 2 == 0) {
            rp->play(deckAfter);
        } else {
            lp->play(deckAfter);
        }
    }
}

void Player::players(unsigned int num) {
    numPlayers = num;
}

void Player::start(Player &lp, Player &rp) {
    Player::lp = &lp;
    Player::rp = &rp;

    resume();
}

void Player::play(unsigned int deck) {
    Player::deck = deck;

    resume();
}

Printer::Printer(const unsigned int numPlayers, const unsigned int numCards)
        : numPlayers(numPlayers), numCards(numCards) {
    info = new PlayerInfo*[numPlayers];
    for (unsigned int i = 0; i < numPlayers; ++i) {
        info[i] = NULL;
    }

    // print game info
    std::cout << "Players: " << numPlayers << "    ";
    std::cout << "Cards: " << numCards << std::endl;

    // print column headers
    for (unsigned int i = 0; i < numPlayers; ++i) {
        std::cout << "P" << i << '\t';
    }
    std::cout << std::endl;
}

Printer::~Printer() {
    delete info;
}

void Printer::flush(unsigned int id /* = Printer::NO_ID */) {
    for (unsigned int i = 0; i < numPlayers; ++i) {
        if (info[i]) {
            std::cout << *info[i];
            if (id == i) {
                // If this user caused the flush, print "*"
                std::cout << "*";
            }

            // Clear already-printed PlayerInfo
            delete info[i];
            info[i] = NULL;
        }
        std::cout << "\t";
    }
    std::cout << std::endl;
}

void Printer::prt(const unsigned int id, const unsigned int took,
                  unsigned int playersRemaining) {
    // If trying to overwrite data, flush
    if (info[id]) {
        flush(id);
    }

    // Played dies if they receive a death deck
    bool dead = !(numCards % Player::DEATH_DECK_DIVISOR);
    numCards -= took;
    // Store important info to-be-printed
    info[id] = new PlayerInfo(took, numCards, !(numCards % 2), dead,
                              playersRemaining == 1);

    // Flush if the game has ended / will immediately end
    if (playersRemaining == 1 || numCards == 0) {
        flush();
    }
}

void uMain::main() {
    int games = 5;
    int seed = getpid();

    switch (argc) {
        default:
            std::cerr << "Usage: ./cardgame [games [seed]]" << std::endl;
            exit(-1);
        case 3:
            seed = std::stoi(argv[2]);
            if (seed <= 0) {
                std::cerr << "Error: seed value must be positive.";
                std::cerr << std::endl;
                exit(-1);
            }
        case 2:
            games = std::stoi(argv[1]);
            if (games < 0) {
                std::cerr << "Error: games value must be positive.";
                std::cerr << std::endl;
                exit(-1);
            }
        case 1:
            break;
    }

    prng = new PRNG(seed);

    for (int game = 0; game < games; ++game) {
        unsigned int numPlayers = (*prng)(2, 10);
        Player::players(numPlayers);

        unsigned int numCards = (*prng)(10, 200);
        Printer printer(numPlayers, numCards);

        Player *players[numPlayers];
        for (unsigned int i = 0; i < numPlayers; ++i) {
            players[i] = new Player(printer, i);
        }

        for (unsigned int i = 0; i < numPlayers; ++i) {
            players[i]->start(*players[(i + numPlayers - 1) % numPlayers],
                              *players[(i + 1) % numPlayers]);
        }

        unsigned int firstPlayer = (*prng)(0, numPlayers - 1);
        players[firstPlayer]->play(numCards);

        for (unsigned int i = 0; i < numPlayers; ++i) {
            delete players[i];
        }

        if (game != games - 1) {
            std::cout << std::endl << std::endl;
        }
    }

    delete prng;
}
