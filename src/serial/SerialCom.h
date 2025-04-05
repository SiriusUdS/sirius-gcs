#ifndef SERIALCOM_H
#define SERIALCOM_H

#include "Logging.h"
#include "RecvBuffer.hpp"

#include <ceserial.h>
#include <optional>

class SerialCom {
public:
    void start();
    bool comOpened();
    void readChar();
    size_t getPacket(char* recv);
    bool write(char* msg, size_t size);
    void shutdown();

private:
    ceSerial com;
    RecvBuffer<10000> recvBuf;
};

#endif // SERIALCOM_H
