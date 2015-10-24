// Pseudo Random Number Generator : generate repeatable sequence of values that
//   appear random by scrambling the bits of a 32-bit integer value.
//
// Interface :
//   PRNG(s) - set starting seed
//   seed() - read seed
//   seed(s) - reset seed
//   prng() - generate random value in range [0,UINT_MAX]
//   prng(u) - generate random value in range [0,u]
//   prng(l,u) - generate random value in range [l,u]
//
// Examples : generate random number between 5-21
//   prng() % 17 + 5    values 0-16 + 5 = 5-21
//   prng(16) + 5
//   prng(5, 21)

#ifndef __PRNG_H__
#define __PRNG_H__

#include <cassert>
#include <stdint.h>

class PRNG {
    uint32_t seed_;

public:
    PRNG(uint32_t s = 362436069) {
        seed_ = s;
        assert(((void)"invalid seed", seed_ != 0));
    }

    uint32_t seed() {
        return seed_;
    }

    void seed(uint32_t s) {
        seed_ = s;
        assert(((void)"invalid seed", seed_ != 0));
    }

    uint32_t operator()() {
        seed_ = 36969 * (seed_ & 65535) + (seed_ >> 16);
        return seed_;
    }

    uint32_t operator()(uint32_t u) {
        assert(((void)"invalid random range", u < (uint32_t)-1));
        return operator()() % (u + 1);
    }
    uint32_t operator()(uint32_t l, uint32_t u) {
        assert(((void)"invalid random range", l <= u));
        return operator()(u - l) + l;
    }
};

#endif
