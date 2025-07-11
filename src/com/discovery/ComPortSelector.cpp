#include "ComPortSelector.h"

#include "ComDiscovery.h"

ComPortSelector::ComPortSelector(ComDiscovery& comDiscovery) : comDiscovery(comDiscovery) {
}

std::string ComPortSelector::current() const {
    if (currentComPortIdx >= comPorts.size()) {
        return "";
    }
    return comPorts[currentComPortIdx];
}

void ComPortSelector::next() {
    if (currentComPortIdx >= comPorts.size()) {
        comDiscovery.getAvailableComPorts(comPorts);
        currentComPortIdx = 0;
    } else {
        currentComPortIdx++;
    }
}
