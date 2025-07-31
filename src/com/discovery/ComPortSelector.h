#ifndef COMPORTSELECTOR_H
#define COMPORTSELECTOR_H

#include <mutex>
#include <string>
#include <vector>

class AbstractComPortDiscovery;

/**
 * @class ComPortSelector
 * @brief Selects available COM ports for communication.
 */
class ComPortSelector {
public:
    ComPortSelector(AbstractComPortDiscovery& comPortDiscovery);
    std::string current() const;
    void next();
    bool available() const;

private:
    AbstractComPortDiscovery& comPortDiscovery;
    size_t currentComPortIdx{};
    std::vector<std::string> comPorts;
    mutable std::mutex mtx;
};

#endif // COMPORTSELECTOR_H
