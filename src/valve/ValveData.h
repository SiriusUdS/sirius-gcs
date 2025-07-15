#ifndef VALVEDATA_H
#define VALVEDATA_H

struct ValveData {
    const char* name;
    bool isIdle{};
    bool closedSwitchHigh{};
    bool openedSwitchHigh{};
};

#endif // VALVEDATA_H
