#ifndef Q1BINSERTSORT_H
#define Q1BINSERTSORT_H

template<typename T> _Coroutine Binsertsort {
    const T Sentinel;
    T value;

    void main();

public:
    Binsertsort(T Sentinel);

    void sort(T value);
    T retrieve();
};

#endif
