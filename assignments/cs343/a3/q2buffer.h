#ifndef Q2BUFFER_H
#define Q2BUFFER_H

#include <cstdlib>
#include <queue>

template<typename T> class BoundedBuffer {
    const unsigned int size;

    std::queue<T> elements;

    uOwnerLock lock;
    uCondLock full;
    uCondLock empty;

#ifdef NOBUSY
    uOwnerLock consumerLock;
    uOwnerLock producerLock;
#endif

public:
    BoundedBuffer(const unsigned int size = 10);

    void insert(T elem);
    T remove();
};

_Task Consumer {
    BoundedBuffer<BTYPE> &buffer;
    const int delay;
    const BTYPE sentinel;
    BTYPE &sum;

    void main();

public:
    Consumer(BoundedBuffer<BTYPE> &buffer, const int delay,
             const BTYPE sentinel, BTYPE &sum);
};

_Task Producer {
    BoundedBuffer<BTYPE> &buffer;
    const int produce;
    const int delay;

    void main();

public:
    Producer(BoundedBuffer<BTYPE> &buffer, const int produce, const int delay);
};

#endif
