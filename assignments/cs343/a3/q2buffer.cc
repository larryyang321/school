#include <iostream>

#include "MPRNG.h"
#include "q2buffer.h"

template<typename T>
BoundedBuffer<T>::BoundedBuffer(const unsigned int size) : size(size) { }

template<typename T>
void BoundedBuffer<T>::insert(T elem) {
#ifdef NOBUSY
    producerLock.acquire();
#endif
    lock.acquire();

#ifdef NOBUSY
    if (elements.size() == size) {
        full.wait(lock);
    }
#elif BUSY
    while (elements.size() == size) {
        full.wait(lock);
    }
#endif

    assert(elements.size() < size);
    elements.push(elem);
    empty.signal();

    lock.release();
#ifdef NOBUSY
    producerLock.release();
#endif
}

template<typename T>
T BoundedBuffer<T>::remove() {
#ifdef NOBUSY
    consumerLock.acquire();
#endif
    lock.acquire();


#ifdef NOBUSY
    if (elements.empty()) {
        empty.wait(lock);
    }
#elif BUSY
    while (elements.empty()) {
        empty.wait(lock);
    }
#endif

    assert(!elements.empty());
    T element = elements.front();
    elements.pop();
    full.signal();

    lock.release();
#ifdef NOBUSY
    consumerLock.release();
#endif

    return element;
}

MPRNG *mprng;

Consumer::Consumer(BoundedBuffer<BTYPE> &buffer, const int delay,
                   const BTYPE sentinel, BTYPE &sum)
        : buffer(buffer), delay(delay), sentinel(sentinel), sum(sum) { }

void Consumer::main() {
    BTYPE element;
    for (;;) {
        yield((*mprng)(delay - 1));

        element = buffer.remove();
        if (element == sentinel) {
            break;
        }

        sum += element;
    }
}

Producer::Producer(BoundedBuffer<BTYPE> &buffer, const int produce,
                   const int delay)
        : buffer(buffer), produce(produce), delay(delay) { }

void Producer::main() {
    for (int i = 1; i <= produce; ++i) {
        yield((*mprng)(delay - 1));

        buffer.insert(BTYPE(i));
    }
}

void uMain::main() {
    int cons = 5, prods = 3, produce = 10, buffersize = 10, delay = -1;

    switch (argc) {
        case 6:
            delay = std::stoi(argv[5]);
            if (delay <= 0) {
                std::cerr << "Error: delay value must be non-negative.";
                std::cerr << std::endl;
                exit(-1);
            }
        case 5:
            buffersize = std::stoi(argv[4]);
            if (buffersize <= 0) {
                std::cerr << "Error: buffersize value must be non-negative.";
                std::cerr << std::endl;
                exit(-1);
            }
        case 4:
            produce = std::stoi(argv[3]);
            if (produce <= 0) {
                std::cerr << "Error: produce value must be non-negative.";
                std::cerr << std::endl;
                exit(-1);
            }
        case 3:
            prods = std::stoi(argv[2]);
            if (prods <= 0) {
                std::cerr << "Error: producers value must be non-negative.";
                std::cerr << std::endl;
                exit(-1);
            }
        case 2:
            cons = std::stoi(argv[1]);
            if (cons <= 0) {
                std::cerr << "Error: consumers value must be non-negative.";
                std::cerr << std::endl;
                exit(-1);
            }
        case 1:
            break;
        default:
            std::cerr << "Usage:" << std::endl;
            std::cerr << "buffer [consumers [producers [produce [buffersize ";
            std::cerr << "[delay]]]]]" << std::endl;
            exit(-1);
    }

    if (delay == -1) {
        delay = cons + prods;
    }

    #ifdef __U_MULTI__
        uProcessor p[3] __attribute__(( unused ));
    #endif

    mprng = new MPRNG();

    BoundedBuffer<BTYPE> buffer(buffersize);

    Producer *producers[prods];
    for (int i = 0; i < prods; ++i) {
        producers[i] = new Producer(buffer, produce, delay);
    }

    BTYPE sum = 0;
    Consumer *consumers[cons];
    for (int i = 0; i < cons; ++i) {
        consumers[i] = new Consumer(buffer, delay, SENTINEL, sum);
    }

    for (int i = 0; i < prods; ++i) {
        delete producers[i];
    }

    for (int i = 0; i < cons; ++i) {
        buffer.insert(SENTINEL);
    }

    for (int i = 0; i < cons; ++i) {
        delete consumers[i];
    }

    std::cout << "total: " << sum << std::endl;

    delete mprng;
}
