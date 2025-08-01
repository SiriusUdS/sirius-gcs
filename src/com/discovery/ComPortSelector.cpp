#include "ComPortSelector.h"

#include "AbstractComPortDiscovery.h"

/**
 * @brief Constructs a ComPortSelector with the given com port discovery instance.
 * @param comPortDiscovery The instance of AbstractComPortDiscovery used to discover available COM ports.
 */
ComPortSelector::ComPortSelector(AbstractComPortDiscovery& comPortDiscovery) : comPortDiscovery(comPortDiscovery) {
}

/**
 * @brief Returns the currently selected COM port.
 * @returns The path of the currently selected COM port.
 */
std::string ComPortSelector::current() const {
    std::lock_guard<std::mutex> lock(mtx);

    if (currentComPortIdx >= comPorts.size()) {
        return "";
    }
    return comPorts[currentComPortIdx];
}

/**
 * @brief Advances to the next COM port in the list.
 */
void ComPortSelector::next() {
    std::lock_guard<std::mutex> lock(mtx);

    currentComPortIdx++;
    if (currentComPortIdx >= comPorts.size()) {
        comPortDiscovery.getAvailableComPorts(comPorts);
        currentComPortIdx = 0;
    }
}

/**
 * @brief Checks if there are any available COM ports.
 * @returns True if there are available COM ports, false otherwise.
 */
bool ComPortSelector::available() const {
    return comPorts.size();
}
