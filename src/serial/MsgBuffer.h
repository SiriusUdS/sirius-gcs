#ifndef MSGBUFFER_H
#define MSGBUFFER_H

#include "Constants.h"

#include <optional>
#include <queue>

class MsgBuffer {
public:
    size_t readPacket(char* rcv);
    bool writeChar(char c);
    bool canReadPacket();
    bool canWriteChar();
    int availablePackets();

private:
    struct PacketInfo {
        int headerCode;
        size_t size;
    };

    std::optional<int> searchAnyHeader(size_t idx);
    bool searchSpecificHeader(int headerCode, size_t idx);

    size_t readCapacity();
    size_t writeCapacity();

    size_t nextIndex(size_t idx, size_t increment = 1);
    size_t prevIndex(size_t idx, size_t decrement = 1);

    size_t readIdx{};
    size_t writeIdx{};
    char buf[Constants::MSG_BUF_SIZE]{};
    bool bufFull{};
    bool writingValidPacket{};
    PacketInfo currPacket;
    std::queue<PacketInfo> availablePacketInfoQueue{};
};

#endif // MSGBUFFER_H
