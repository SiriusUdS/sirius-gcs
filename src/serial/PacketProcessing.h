#ifndef PACKETPROCESSING_H
#define PACKETPROCESSING_H

#include "PlotData.h"
#include "SerialCom.h"

#include <stdint.h>

namespace PacketProcessing {
bool processIncomingPacket();

bool processAccelerometerPacket();
bool processGyroscopePacket();
bool processAltimeterPacket();
bool processGpsPacket();
bool processMagnetometerPacket();
bool processPressureSensorPacket();
bool processRocketPacket();
bool processTemperatureSensorPacket();
bool processValvePacket();

bool validateIncomingPacketSize(size_t targetPacketSize, const char* packetName);

static float voltageConverter_V(float adcValue);
static float pressureConverter_NAME1_PSI(float ADCInput, uint8_t index);
} // namespace PacketProcessing

#endif // PACKETPROCESSING_H
