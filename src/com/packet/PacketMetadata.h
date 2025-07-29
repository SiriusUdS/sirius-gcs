#ifndef PACKETMETADATA_H
#define PACKETMETADATA_H

struct PacketMetadata {
    enum class Status { NONE, VALID, INVALID, DUMP_IMMEDIATLY };
    Status status{Status::NONE};
    size_t size{};
};

#endif // PACKETMETADATA_H
