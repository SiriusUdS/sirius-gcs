#ifndef SERIALCOM_H
#define SERIALCOM_H

#include "Constants.h"
#include "Logging.h"
#include "RecvBuffer.hpp"

#include <ceserial.h>
#include <optional>

/**
 * @class SerialCom
 * @brief Handles serial communication to receive packets
 */
class SerialCom {
public:
    void start();
    bool comOpened();
    void read();
    size_t getPacket(uint8_t* recv);
    uint32_t nextPacketHeaderCode();
    size_t nextPacketSize();
    bool write(uint8_t* msg, size_t size);
    void shutdown();

private:
    ceSerial com;
    RecvBuffer<Constants::RECV_BUF_SIZE> recvBuf;
};

#endif // SERIALCOM_H
