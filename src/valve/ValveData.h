#ifndef VALVEDATA_H
#define VALVEDATA_H

#include <cstdint>

#include "ValveStateData.h"

struct ValveData {
    ValveStateData valveStateData;
    uint8_t id;
    const char* name;
    int openedValue_perc;
    int lastOpenedValue_perc;
};

#endif // VALVEDATA_H
