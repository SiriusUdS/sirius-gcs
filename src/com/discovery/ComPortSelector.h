#ifndef COMPORTSELECTOR_H
#define COMPORTSELECTOR_H

#include "ComDiscovery.h"

#include <mutex>
#include <string>
#include <vector>

class ComPortSelector {
public:
    ComPortSelector(ComDiscovery& comDiscovery);
    std::string current() const;
    void next();

private:
    ComDiscovery& comDiscovery;
    size_t currentComPortIdx{};
    std::vector<std::string> comPorts;
    mutable std::mutex mtx;
};

#endif // COMPORTSELECTOR_H
