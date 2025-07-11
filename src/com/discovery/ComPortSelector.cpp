#include "ComPortSelector.h"

#include "ComDiscovery.h"

const std::string& ComPortSelector::current() const {
    if (currentComPortIdx >= comPorts.size()) {
        return "";
    }
    return comPorts[currentComPortIdx];
}

void ComPortSelector::next() {
    if (currentComPortIdx >= comPorts.size()) {
        ComDiscovery::getAvailableComPorts(comPorts);
        currentComPortIdx = 0;
    } else {
        currentComPortIdx++;
    }
}
