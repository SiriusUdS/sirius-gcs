#include "SerialCom.h"

#include "ComDiscovery.h"

void SerialCom::start() {
    if (com.IsOpened()) {
        com.Close();
    }

    std::vector<std::string> availableComPorts;
    getAvailableComPorts(availableComPorts);
    if (availableComPorts.empty()) {
        return;
    }

    std::string comPath = std::string("\\\\.\\") + availableComPorts[0];
    com.SetPortName(comPath);
    com.Open();
}

bool SerialCom::comOpened() {
    return com.IsOpened();
}

void SerialCom::readChar() {
    if (!com.IsOpened()) {
        return;
    }

    bool successFlag;
    char c = com.ReadChar(successFlag);
    if (successFlag) {
        recvBuf.writeChar(c);
    }
}

size_t SerialCom::getPacket(char* recv) {
    return recvBuf.readPacket(recv);
}

bool SerialCom::write(char* msg, size_t size) {
    if (!com.IsOpened()) {
        return false;
    }

    return com.Write(msg, (long) size);
}

void SerialCom::shutdown() {
    com.Close();
}
