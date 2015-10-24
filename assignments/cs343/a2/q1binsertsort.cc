#include <fstream>
#include <iostream>

#include "q1binsertsort.h"

// -----------------------------------------------------------
// Sample Type for proving this works with user-defined types.
//
// You can use this type by running, for example:
//     make TYPE="SampleType" SENTINEL="SampleType(-1, -1)"
struct SampleType {
    int x;
    int y;

    SampleType() { }
    SampleType(int x, int y) : x(x), y(y) { }
};

bool operator==(const SampleType& l, const SampleType& r) {
    return (l.x == r.x && l.y == r.y);
}

bool operator<=(const SampleType& l, const SampleType& r) {
    return (l.x <= r.x && l.y <= r.y);
}

std::istream& operator>>(std::istream& is, SampleType& obj) {
    is >> obj.x;
    obj.y = 42;
    return is;
}

std::ostream& operator<<(std::ostream& os, const SampleType& obj) {
    os << "[" << obj.x << ":" << obj.y << "]";
    return os;
}
// -----------------------------------------------------------

template<typename T>
void Binsertsort<T>::main() {
    Binsertsort<T> less(Sentinel);
    Binsertsort<T> greater(Sentinel);

    T pivot = value;
    if (value == Sentinel) {
        suspend();
        return;
    }

    for (;;) {
        suspend();

        // When a Sentinel value is received, sort it into all children
        if (value == Sentinel) {
            less.sort(Sentinel);
            do {
                suspend();
                value = less.retrieve();
            } while (!(value == Sentinel));

            // Set communication variable to original value
            value = pivot;

            greater.sort(Sentinel);
            do {
                suspend();
                value = greater.retrieve();
            } while (!(value == Sentinel));

            return;
        }

        // Sort on lhs or rhs
        if (value <= pivot) {
            less.sort(value);
        } else {
            greater.sort(value);
        }
    }
}

template<typename T>
Binsertsort<T>::Binsertsort(T Sentinel) : Sentinel(Sentinel) {}

template<typename T>
void Binsertsort<T>::sort(T value) {
    Binsertsort::value = value;
    resume();
}

template<typename T>
T Binsertsort<T>::retrieve() {
    resume();
    return value;
}

// Much IO code taken from
// https://www.student.cs.uwaterloo.ca/~cs343/examples/uIO.cc
void uMain::main() {
    std::istream *input;
    std::ostream *output = &std::cout;

    switch (argc) {
        case 3:
            try {
                output = new std::ofstream(argv[2]);
            } catch (uFile::Failure) {
                std::cerr << "Could not open output file '" << argv[2] << "'.";
                std::cerr << std::endl;
                exit(-1);
            }
        case 2:
            try {
                input = new std::ifstream(argv[1]);
            } catch (uFile::Failure) {
                std::cerr << "Could not open input file '" << argv[1] << "'.";
                std::cerr << std::endl;
                exit(-1);
            }

            break;
        default:
            std::cerr << "Usage: ./binsertsort input [output]" << std::endl;
            exit(-1);
    }

    int count;
    for (;;) {
        *input >> count;
        if (!*input) {
            break;
        }

        Binsertsort<TYPE> binsertsort(SENTINEL);
        TYPE val;
        for (int i = 0; i < count; ++i) {
            *input >> val;
            if (!*input) {
                std::cerr << std::endl;
                std::cerr << "Error: line terminated early." << std::endl;
                exit(-1);
            }

            *output << val;
            if (i < count - 1) {
                *output << " ";
            }

            binsertsort.sort(val);
        }
        *output << std::endl;

        binsertsort.sort(SENTINEL);

        for (int i = 0; i < count; ++i) {
            *output << binsertsort.retrieve();
            if (i < count - 1) {
                *output << " ";
            }
        }
        *output << std::endl << std::endl;
    }

    if (input != &std::cin) {
        delete input;
    }
    if (output != &std::cout) {
        delete output;
    }
}
