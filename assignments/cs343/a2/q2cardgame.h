#ifndef Q2CARDGAME_H
#define Q2CARDGAME_H

struct PlayerInfo {
    unsigned int took;
    unsigned int deck;
    bool pass_right;
    bool dead;
    bool last;

    PlayerInfo(unsigned int took, unsigned int deck, bool pass_right,
               bool dead, bool last) :
            took(took), deck(deck), pass_right(pass_right), dead(dead),
            last(last) { }
};

class Printer {
    // Default variable for Printer::flush (no users can have an ID this large)
    const static unsigned int NO_ID = 9999;

    const unsigned int numPlayers;
    unsigned int numCards;

    PlayerInfo **info;

    void flush(unsigned int id = Printer::NO_ID);

public:
    Printer(const unsigned int numPlayers, const unsigned int numCards);
    ~Printer();

    void prt(const unsigned int id, const unsigned int took,
             unsigned int playersRemaining);
};

_Coroutine Player {
    static unsigned int numPlayers;

    Printer &printer;

    unsigned int id;
    unsigned int deck;

    Player *lp;
    Player *rp;

public:
    enum {
        DEATH_DECK_DIVISOR = 5
    };

    Player(Printer &printer, unsigned int id);

    void main();
    static void players(unsigned int num);
    void start(Player &lp, Player &rp);
    void play(unsigned int deck);
};

unsigned int Player::numPlayers;

#endif
