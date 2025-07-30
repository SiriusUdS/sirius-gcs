#ifndef VALVEDATA_H
#define VALVEDATA_H

struct ValveData {
    bool isIdle{};
    bool closedSwitchHigh{};
    bool openedSwitchHigh{};
    uint16_t positionOpened_pct{};
};

#endif // VALVEDATA_H
