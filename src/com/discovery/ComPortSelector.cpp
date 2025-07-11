#include "ComPortSelector.h"

#include "ComDiscovery.h"

ComPortSelector::ComPortSelector(ComDiscovery& comDiscovery) : comDiscovery(comDiscovery) {
}

std::string ComPortSelector::current() const {
    std::lock_guard<std::mutex> lock(mtx);

    if (currentComPortIdx >= comPorts.size()) {
        return "";
    }
    return comPorts[currentComPortIdx];
}

void ComPortSelector::next() {
    std::lock_guard<std::mutex> lock(mtx);

    currentComPortIdx++;
    if (currentComPortIdx >= comPorts.size()) {
        comDiscovery.getAvailableComPorts(comPorts);
        currentComPortIdx = 0;
    }
}
