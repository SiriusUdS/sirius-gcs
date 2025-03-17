#ifndef MSGBUFFER_H
#define MSGBUFFER_H

#include "Constants.h"

#include <optional>

class MsgBuffer {
public:
    size_t readPacket(char* rcv);
    bool write(char* msg, size_t size);
    bool canRead(size_t size);
    bool canWrite(size_t size);

private:
    std::optional<std::pair<int, size_t>> searchAnyHeader(size_t idx);
    std::optional<std::pair<int, size_t>> searchAnyHeader(size_t startIdx, size_t endIdx);
    std::optional<size_t> searchHeader(int headerCode, size_t startIdx, size_t endIdx);

    size_t readCapacity();
    size_t writeCapacity();
    size_t readCapacityFromIdx(size_t idx);
    size_t writeCapacityFromIdx(size_t idx);

    size_t nextIndex(size_t idx, size_t increment = 1);
    void advanceIndex(size_t& idx, size_t increment = 1);

    size_t readIdx{};
    size_t writeIdx{};
    char buf[Constants::MSG_BUF_SIZE]{};
    bool bufFull{};
    int nbPacketsReady{};
};

#endif // MSGBUFFER_H
