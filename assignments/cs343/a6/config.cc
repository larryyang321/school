#include <fstream>
#include <iostream>
#include <limits>

#include "config.h"

using namespace std;

static bool comments(ifstream &in, string &name) {
    for (;;) {
        in >> name;
        if (in.fail()) {
            return true;
        }
        if (name.substr(0, 1) != "#") {
            break;
        }

        in.ignore(numeric_limits<int>::max(), '\n');
    }

    return false;
}

void processConfigFile(const char *configFile, ConfigParams &cparams) {
    const unsigned int paramNum = 10;

    struct {
        const char *name;
        bool used;
        unsigned int &value;
    } static params[paramNum] = {
        { "SodaCost", false, cparams.sodaCost },
        { "NumStudents", false, cparams.numStudents },
        { "MaxPurchases", false, cparams.maxPurchases },
        { "NumVendingMachines", false, cparams.numVendingMachines },
        { "MaxStockPerFlavour", false, cparams.maxStockPerFlavour },
        { "MaxShippedPerFlavour", false, cparams.maxShippedPerFlavour },
        { "TimeBetweenShipments", false, cparams.timeBetweenShipments },
        { "GroupoffDelay", false, cparams.groupoffDelay },
        { "ParentalDelay", false, cparams.parentalDelay },
        { "NumCouriers", false, cparams.numCouriers },
    };

    string name;
    int value;
    unsigned int cnt, posn;
    unsigned int numOfParam = 0;

    try {
        ifstream in(configFile);

        for (cnt = 0 ; cnt < paramNum; ++cnt) {
            if (comments(in, name)) {
                break;
            }
            for (posn = 0; posn < paramNum && name != params[posn].name; ++posn);

            if (posn == paramNum) {
                break;
            }
            if (params[posn].used) {
                break;
            }

            in >> value;
            if (value <= 0) {
                cerr << "Error: file \"" << configFile << "\" parameter " << name
                     << " value " << value << " must be positive." << endl;
                exit(EXIT_FAILURE);
            }
            if (in.fail()) {
                break;
            }

            in.ignore(numeric_limits<int>::max(), '\n');

            ++numOfParam;
            params[posn].used = true;
            params[posn].value = value;
        }

        if (numOfParam != paramNum) {
            cerr << "Error: file \"" << configFile << "\" is corrupt." << endl;
            exit(EXIT_FAILURE);
        }
        if (!comments(in, name)) {
            cerr << "Error: file \"" << configFile << "\" has extraneous data." << endl;
            exit(EXIT_FAILURE);
        }
    } catch(uFile::Failure) {
        cerr << "Error: could not open input file \"" << configFile << "\"" << endl;
        exit(EXIT_FAILURE);
    }
}
