#ifndef SERIALCOM_H
#define SERIALCOM_H

#include "Logging.h"
#include "RecvBuffer.hpp"

#include <Windows.h>
#include <ceserial.h>
#include <optional>

class SerialCom {
public:
    void init();
    void getAvailableComPorts();
    void readChar();
    size_t getPacket(char* recv);
    bool write(char* msg, size_t size);
    void shutdown();

private:
    ceSerial com;
    RecvBuffer<10000> recvBuf;
    std::vector<std::string> availableComPorts;
};

#endif // SERIALCOM_H
