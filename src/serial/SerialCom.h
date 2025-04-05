#ifndef SERIALCOM_H
#define SERIALCOM_H

#include "Constants.h"
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
    RecvBuffer<Constants::RECV_BUF_SIZE> recvBuf;
};

#endif // SERIALCOM_H
