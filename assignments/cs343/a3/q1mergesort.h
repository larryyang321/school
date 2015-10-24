#ifndef Q1MERGESORT_H
#define Q1MERGESORT_H

template<typename T> _Task Mergesort {
    T *values;
    unsigned int low;
    unsigned int high;
    unsigned int depth;

    T *unsorted;

    void main();
    void sort();

    Mergesort(T values[], unsigned int low, unsigned int high,
              unsigned int depth, T unsorted[]);
public:
    Mergesort(T values[], unsigned int low, unsigned int high,
              unsigned int depth);
    ~Mergesort();
};

unsigned int uDefaultStackSize() {
    return 512 * 1000;
}

#endif
