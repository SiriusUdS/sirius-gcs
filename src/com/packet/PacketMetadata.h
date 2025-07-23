#ifndef PACKETMETADATA_H
#define PACKETMETADATA_H

struct PacketMetadata {
    size_t size;
    bool isValid;
    uint32_t packetTypeCode;
    uint32_t boardId;
};

#endif // PACKETMETADATA_H
