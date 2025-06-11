#ifndef PACKETPROCESSING_H

#include "Constants.h"

#include <stdint.h>

namespace PacketProcessing {
bool processIncomingPacket();
bool processTelemetryPacket();
bool processGSControlPacket();
bool processEngineStatusPacket();
bool validateIncomingPacketSize(size_t targetPacketSize, const char* packetName);
bool dumpNextPacket(const char* packetName);

extern size_t packetSize;
extern uint8_t packetBuf[Constants::RECV_PACKET_MAX_SIZE];
} // namespace PacketProcessing

#endif // PACKETPROCESSING_H
