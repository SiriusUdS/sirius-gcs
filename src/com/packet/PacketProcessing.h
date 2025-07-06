#ifndef PACKETPROCESSING_H

#include <stdint.h>

namespace PacketProcessing {
void processIncomingPackets();
bool processIncomingPacket();
bool processEngineTelemetryPacket();
bool processGSControlPacket();
bool processEngineStatusPacket();
bool validateIncomingPacketSize(size_t targetPacketSize, const char* packetName);
bool dumpNextPacket(const char* packetName);
} // namespace PacketProcessing

#endif // PACKETPROCESSING_H
