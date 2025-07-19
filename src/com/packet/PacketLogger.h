#ifndef PACKETLOGGER_H
#define PACKETLOGGER_H

#include "GSControl/GSControlStatus.h"
#include "Valve/ValveStatus.h"

namespace PacketLogger {
void logEngineTelemetry(uint16_t* thermistorAdcValues, float* thermistorValues, uint16_t* pressureSensorAdcValues, float* pressureSensorValues);
void logFillingStationTelemetry(uint16_t* thermistorAdcValues,
                                float* thermistorValues,
                                uint16_t* pressureSensorAdcValues,
                                float* pressureSensorValues,
                                uint16_t* loadCellAdcValues,
                                float* loadCellValues);
void logGsControl(GSControlStatus& packet);
void logEngineStatus(ValveStatus& nosValveStatus, ValveStatus& ipaValveStatus);
void logFillingStationStatus(ValveStatus& fillValveStatus, ValveStatus& dumpValveStatus);
} // namespace PacketLogger

#endif // PACKETLOGGER_H
