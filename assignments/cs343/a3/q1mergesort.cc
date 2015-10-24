#include <fstream>
#include <iostream>

#include "q1mergesort.h"

template<typename T>
void Mergesort<T>::main() {
    sort();
}

template<typename T>
void Mergesort<T>::sort() {
    if (high - low == 1) {
        return;
    }

    unsigned int pivot = (low + high) / 2;
    {
        if (depth > 0) {
            Mergesort<T> rhs(values, pivot, high, depth - 1);
        } else {
            unsigned int currentLow = low;
            low = pivot;
            sort();
            low = currentLow;
        }

        unsigned int currentHigh = high;
        high = pivot;
        if (depth > 0) {
            --depth;
        }
        sort();
        high = currentHigh;
    }

    T *ptr = &unsorted[low];
    for (unsigned int i = 0, li = low, ri = pivot; i < high - low;
         ++i, ++ptr) {
        if ((li < pivot && values[li] < values[ri]) || ri == high) {
            *ptr = values[li++];
        } else {
            *ptr = values[ri++];
        }
    }

    memcpy(&values[low], &unsorted[low], sizeof(T) * (high - low));
}

template<typename T>
Mergesort<T>::Mergesort(T values[], unsigned int low, unsigned int high,
                        unsigned int depth)
        : values(values), low(low), high(high), depth(depth) {
    unsortedOwner = (unsorted == NULL);
    if (unsortedOwner) {
        unsorted = new T[high - low];
    }
}

template<typename T>
Mergesort<T>::~Mergesort() {
    if (unsortedOwner) {
        delete[] unsorted;
    }
}

void usage() {
    std::cerr << "Usage:" << std::endl;
    std::cerr << "mergesort -s input-file [output-file]" << std::endl;
    std::cerr << "mergesort -t size [depth]" << std::endl;
}

void uMain::main() {
    if (argc < 2) {
        usage();
        exit(-1);
    }

    if (strcmp(argv[1], "-s") == 0) {
        std::istream *input = &std::cin;
        std::ostream *output = &std::cout;

        switch (argc) {
            case 4:
                try {
                    output = new std::ofstream(argv[3]);
                } catch (uFile::Failure) {
                    std::cerr << "Could not open output file '" << argv[2];
                    std::cerr << "'." << std::endl;
                    exit(-1);
                }
            case 3:
                try {
                    input = new std::ifstream(argv[2]);
                } catch (uFile::Failure) {
                    std::cerr << "Could not open input file '" << argv[1];
                    std::cerr << "'." << std::endl;
                    exit(-1);
                }

                break;
            default:
                usage();
                exit(-1);
        }

        unsigned int count;
        for (;;) {
            *input >> count;
            if (!*input) {
                break;
            }

            TYPE *values = new TYPE[count];
            for (unsigned int i = 0; i < count; ++i) {
                *input >> values[i];
                *output << values[i] << " ";
            }
            *output << std::endl;

            {
                Mergesort<TYPE> mergesort(values, 0, count, 0);
            }

            for (unsigned int i = 0; i < count; ++i) {
                *output << values[i] << " ";
            }
            *output << std::endl << std::endl;

            delete[] values;
        }

        if (input != &std::cin) {
            delete input;
        }
        if (output != &std::cout) {
            delete output;
        }
    } else if (strcmp(argv[1], "-t") == 0) {
        int depth = 0, size = 0;

        switch (argc) {
            case 4:
                depth = std::stoi(argv[3]);
                if (depth < 0) {
                    std::cerr << "Error: depth value must be non-negative.";
                    std::cerr << std::endl;
                    exit(-1);
                }
            case 3:
                size = std::stoi(argv[2]);
                if (size < 0) {
                    std::cerr << "Error: size value must be non-negative.";
                    std::cerr << std::endl;
                    exit(-1);
                }

                break;
            default:
                usage();
                exit(-1);
        }

        TYPE *values = new TYPE[size];
        for (int i = 0; i < size; ++i) {
            values[i] = size - i;
        }

        {
            uProcessor p[(1 << depth) - 1] __attribute__(( unused ));
            Mergesort<TYPE> mergesort(values, 0, size, depth);
        }

        delete[] values;
    } else {
        usage();
        exit(-1);
    }
}
