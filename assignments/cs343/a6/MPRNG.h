#ifndef MPRNG_H
#define MPRNG_H

_Monitor MPRNG {
    unsigned int seed_;

public:
    MPRNG(unsigned int seed = 1009) : _seed(seed) {
        srand(seed);
    }

    unsigned int seed() {
        return seed_;
    }

    void seed(unsigned int seed) {
        seed_ = seed;
        srand(seed);
    }

    unsigned int operator()() {
        return rand();
    }
    unsigned int operator()(unsigned int u) {
        return operator()() % (u + 1);
    }
    unsigned int operator()(unsigned int l, unsigned int u) {
        return operator()(u - l) + l;
    }
};

#endif
