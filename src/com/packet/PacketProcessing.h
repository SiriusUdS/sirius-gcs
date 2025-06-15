#ifndef PACKETPROCESSING_H

#include <stdint.h>

namespace PacketProcessing {
bool processIncomingPacket();
bool processTelemetryPacket();
bool processGSControlPacket();
bool processEngineStatusPacket();
bool validateIncomingPacketSize(size_t targetPacketSize, const char* packetName);
bool dumpNextPacket(const char* packetName);
} // namespace PacketProcessing

#endif // PACKETPROCESSING_H
