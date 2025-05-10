#include "SerialTask.h"

#include "PacketProcessing.h"
#include "SerialControl.h"

namespace SerialTask {
SerialCom com;
}

void SerialTask::performTask() {
    // TODO - Add actual condition for this loop
    while (true) {
        SerialControl::startComIfNeeded();
        SerialControl::readIncomingBytesAtSetRate();
        PacketProcessing::processIncomingPacket();
    }
}
