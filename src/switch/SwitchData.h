#ifndef SWITCHDATA_H
#define SWITCHDATA_H

/**
 * @struct SwitchData
 * @brief Represents the state of a switch/button
 */
struct SwitchData {
    const char* name{}; ///< Name of the switch
    bool isOn{};        ///< Whether the switch is on or off
};

#endif // SWITCHDATA_H
