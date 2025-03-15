#ifndef MSGBUFFER_H
#define MSGBUFFER_H

#include <optional>

class MsgBuffer {
public:
    bool read(char* rcv, size_t size);
    bool write(char* msg, size_t size);
    bool canRead(size_t size);
    bool canWrite(size_t size);
    size_t readCapacity();
    size_t writeCapacity();

    std::optional<size_t> searchAnyHeader(size_t startIdx, size_t endIdx);
    std::optional<size_t> searchHeader(int headerCode, size_t startIdx, size_t endIdx);

private:
    size_t readCapacityFromIdx(size_t idx);
    size_t writeCapacityFromIdx(size_t idx);

    size_t readIdx{};
    size_t writeIdx{};
    static const size_t bufSize = 1000; // TODO - Move constant to constant.h later
    char buf[bufSize]{};
    bool bufFull{};
    bool msgReady{};
};

#endif // MSGBUFFER_H
