#ifndef PACKETPROCESSING_H
#define PACKETPROCESSING_H

#include "PlotData.h"
#include "SerialCom.h"

#include <stdint.h>

typedef struct {
    float temperature;
    float resistance;
} RT_Point;

namespace PacketProcessing {
bool processIncomingPacket();

bool processAccelerometerPacket();
bool processGyroscopePacket();
bool processAltimeterPacket();
bool processGpsPacket();
bool processMagnetometerPacket();
bool processPressureSensorPacket();
bool processRocketPacket();
float interpolateTemperature(float measuredResistance, RT_Point* rtTable, int tableSize);
bool processTemperatureSensorPacket();
bool processValvePacket();

bool validateIncomingPacketSize(size_t targetPacketSize, const char* packetName);
} // namespace PacketProcessing

#endif // PACKETPROCESSING_H
