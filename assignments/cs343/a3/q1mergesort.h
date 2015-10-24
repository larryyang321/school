#ifndef Q1MERGESORT_H
#define Q1MERGESORT_H

template<typename T> _Task Mergesort {
    static T *unsorted;
    bool unsortedOwner;

    T *values;
    unsigned int low;
    unsigned int high;
    unsigned int depth;

    void main();
    void sort();

public:
    Mergesort(T values[], unsigned int low, unsigned int high,
              unsigned int depth);
    ~Mergesort();
};

template<typename T>
T *Mergesort<T>::unsorted;

#endif
