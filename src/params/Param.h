#ifndef PARAM_H
#define PARAM_H

#include <string>

template <typename T>
struct Param {
    Param(T defaultValue, std::string iniKey) : currentValue{defaultValue}, defaultValue{defaultValue}, iniKey{iniKey} {
    }

    T currentValue;
    T defaultValue;
    std::string iniKey;
};

#endif // PARAM_H
