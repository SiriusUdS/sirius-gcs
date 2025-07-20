#include "ComPortSelector.h"

#include "AbstractComPortDiscovery.h"

ComPortSelector::ComPortSelector(AbstractComPortDiscovery& comPortDiscovery) : comPortDiscovery(comPortDiscovery) {
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
        comPortDiscovery.getAvailableComPorts(comPorts);
        currentComPortIdx = 0;
    }
}

bool ComPortSelector::available() const {
    return comPorts.size();
}
