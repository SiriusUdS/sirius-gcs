#ifndef SERIALCOM_H
#define SERIALCOM_H

#include "Constants.h"
#include "Logging.h"
#include "RecvBuffer.hpp"

#include <ceserial.h>
#include <chrono>
#include <optional>

/**
 * @class SerialCom
 * @brief Handles serial communication to receive packets
 */
class SerialCom {
public:
    void start();
    bool read();
    bool write(uint8_t* msg, size_t size);
    bool comOpened();
    bool comWorking();
    size_t getPacket(uint8_t* recv);
    uint32_t nextPacketHeaderCode();
    size_t nextPacketSize();
    bool dumpNextPacket();
    size_t packetsReadPerSecond();
    void shutdown();

private:
    size_t consecutiveFailedReads{};
    size_t consecutiveFailedWrites{};
    size_t packetsRead{};
    std::chrono::time_point<std::chrono::steady_clock> comStartTimePoint;
    ceSerial com;
    RecvBuffer<Constants::RECV_BUF_SIZE> recvBuf;
};

#endif // SERIALCOM_H
