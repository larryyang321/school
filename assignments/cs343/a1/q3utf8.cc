#include <fstream>
#include <iostream>

using namespace std;

_Coroutine Utf8 {
private:
    unsigned char ch;

public:
    unsigned int consumed;

    _Event Match {
    public:
        unsigned int unicode;

        Match(unsigned int unicode): unicode(unicode) {}
    };

    _Event Error {};

    Utf8() {
        resume();
    }

    void next(unsigned char c) {
        ch = c;
        resume();
    }

    void main() {
        for(;;) {
            consumed = 0;

            suspend();

            if (ch == '\n') {
                cout << " : Warning! Blank line." << endl;
                continue;
            }

            consumed <<= 8;
            consumed |= ch;

            try {
                // bit 7 == 0 implies ASCII
                if (!(ch & (1 << 7))) {
                    throw Match(ch);
                }

                // bit 6 != 0 implies non-utf8
                if (ch == (ch & ~(1 << 6))) {
                    throw Error();
                }

                // count additional bytes in char
                short utf8bytes = 0;
                unsigned int mask = 1 << 6;
                for (; ch & mask; ++utf8bytes, mask >>= 1) {
                    // utf8 can not have 5+ bytes (1 + utf8bytes)
                    if (utf8bytes > 3) {
                        throw Error();
                    }
                }

                // size of minimal encoding for this char
                unsigned int smallest = 1 << (1 + 5 * utf8bytes);

                // remaining bits
                unsigned int utf8 = ch & (mask - 1);
                for (; utf8bytes > 0; --utf8bytes) {
                    suspend();

                    // Not blank line warning, but early termination
                    if (ch == '\n') {
                        throw Error();
                    }

                    consumed <<= 8;
                    consumed |= ch;

                    // if not of form 10xxxxxx, invalid utf8
                    if (!(ch & (1 << 7)) || !((ch & ~(1 << 6)) == ch)) {
                        throw Error();
                    }

                    // append new data
                    utf8 <<= 6;
                    utf8 |= ch & ((1 << 6) - 1);
                }

                // char was not encoded minimally if utf8 < smallest
                if (utf8 < smallest) {
                    throw Error();
                }

                throw Match(utf8);
            } catch (Match m) {
                _Resume m _At resumer();
            } catch (Error e) {
                // _Resume e _At resumer();
                cout << hex << "0x" << consumed << " : ";
                cout << "invalid";
            }

            if (ch == '\n') {
                cout << endl;
                continue;
            }

            short printedExtra = 0;
            for (;;) {
                suspend();

                if (ch == '\n') {
                    break;
                }

                if (!printedExtra) {
                    printedExtra ^= 1;
                    cout << ". Extra characters 0x";
                }
                cout << hex << (unsigned int)ch;
            }

            cout << endl;
        }
    }
};

// Much IO code taken from
// https://www.student.cs.uwaterloo.ca/~cs343/examples/uIO.cc
void uMain::main() {
    istream *input;

    switch (argc) {
        case 1:
            input = &cin;
            break;
        case 2:
            try {
                input = new ifstream(argv[1]);
            } catch (uFile::Failure) {
                cerr << "Could not open file '" << argv[1] << "'." << endl;
                exit(-1);
            }
            break;
        default:
            cerr << "Usage: ./q3utf8 [input]" << endl;
            exit(-1);
    }

    *input >> noskipws;

    Utf8 utf;

    unsigned char byte;
    for (;;) {
        *input >> byte;
        if (!*input) {
            break;
        }

        try {
            _Enable {
                utf.next(byte);
            }
        } _CatchResume (Utf8::Match m) {
            cout << hex << "0x" << utf.consumed << " : ";
            cout << "valid 0x" << hex << m.unicode;
        } _CatchResume (Utf8::Error) {
            cout << hex << "0x" << utf.consumed << " : ";
            cout << "invalid";
        }
    }

    if (input != &cin) {
        delete input;
    }
}
