#ifndef CONFIG_H
#define CONFIG_H

struct ConfigParams {
    unsigned int sodaCost;
    unsigned int numStudents;
    unsigned int maxPurchases;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
    unsigned int maxShippedPerFlavour;
    unsigned int timeBetweenShipments;
    unsigned int groupoffDelay;
    unsigned int parentalDelay;
    unsigned int numCouriers;
};

void processConfigFile(const char *configFile, ConfigParams &cparams);

#endif
