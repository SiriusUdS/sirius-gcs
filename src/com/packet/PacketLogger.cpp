#include "PacketLogger.h"

#include "Logging.h"

namespace PacketLogger {
// clang-format off
constexpr const char* ENGINE_TELEMETRY_LOG_TEMPLATE =
   "Engine Telemetry Packet -> "
   "Thermistor 1: (ADC: {}, Value: {}), "
   "Thermistor 2: (ADC: {}, Value: {}), "
   "Thermistor 3: (ADC: {}, Value: {}), "
   "Thermistor 4: (ADC: {}, Value: {}), "
   "Thermistor 5: (ADC: {}, Value: {}), "
   "Thermistor 6: (ADC: {}, Value: {}), "
   "Thermistor 7: (ADC: {}, Value: {}), "
   "Thermistor 8: (ADC: {}, Value: {}), "
   "Pressure Sensor 1: (ADC: {}, Value: {}), "
   "Pressure Sensor 2: (ADC: {}, Value: {})";
constexpr const char* FILLING_STATION_TELEMETRY_LOG_TEMPLATE =
   "Filling Station Telemetry Packet -> "
   "Thermistor 1: (ADC: {}, Value: {}), "
   "Thermistor 2: (ADC: {}, Value: {}), "
   "Thermistor 3: (ADC: {}, Value: {}), "
   "Thermistor 4: (ADC: {}, Value: {}), "
   "Thermistor 5: (ADC: {}, Value: {}), "
   "Thermistor 6: (ADC: {}, Value: {}), "
   "Thermistor 7: (ADC: {}, Value: {}), "
   "Thermistor 8: (ADC: {}, Value: {}), "
   "Pressure Sensor 1: (ADC: {}, Value: {}), "
   "Pressure Sensor 2: (ADC: {}, Value: {}), "
   "Load Cell 1: (ADC: {}, Value: {}), "
   "Load Cell 2: (ADC: {}, Value: {})";
constexpr const char* GS_CONTROL_PACKET_LOG_TEMPLATE = 
   "GS Control Packet -> "
   "Allow Dump Switch: {}, "
   "Allow Fill Switch: {}, "
   "Arm Igniter Switch: {}, "
   "Arm Servo Switch: {}, "
   "Emergency Stop Button: {}, "
   "Fire Igniter Button: {}, "
   "Unsafe Key Switch: {}, "
   "Valve Start Button: {}";
constexpr const char* ENGINE_STATUS_PACKET_LOG_TEMPLATE = 
   "Engine Status Packet -> "
   "NOS Valve ("
       "Is Idle: {}, "
       "Closed Switch High: {}, "
       "Opened Switch High: {}"
   "), "
   "IPA Valve ("
       "Is Idle: {}, "
       "Closed Switch High: {}, "
       "Opened Switch High: {}"
   ")";
constexpr const char* FILLING_STATION_STATUS_PACKET_LOG_TEMPLATE = 
   "Filling Station Packet -> "
   "Fill Valve ("
       "Is Idle: {}, "
       "Closed Switch High: {}, "
       "Opened Switch High: {}"
   "), "
   "Dump Valve ("
       "Is Idle: {}, "
       "Closed Switch High: {}, "
       "Opened Switch High: {}"
   ")";
// clang-format on

const char* onOff(bool b);
} // namespace PacketLogger

void PacketLogger::logEngineTelemetry(uint16_t* thermistorAdcValues,
                                      float* thermistorValues,
                                      uint16_t* pressureSensorAdcValues,
                                      float* pressureSensorValues) {
    GCS_DATA_LOG(ENGINE_TELEMETRY_LOG_TEMPLATE,
                 thermistorAdcValues[0],
                 thermistorValues[0],
                 thermistorAdcValues[1],
                 thermistorValues[1],
                 thermistorAdcValues[2],
                 thermistorValues[2],
                 thermistorAdcValues[3],
                 thermistorValues[3],
                 thermistorAdcValues[4],
                 thermistorValues[4],
                 thermistorAdcValues[5],
                 thermistorValues[5],
                 thermistorAdcValues[6],
                 thermistorValues[6],
                 thermistorAdcValues[7],
                 thermistorValues[7],
                 pressureSensorAdcValues[0],
                 pressureSensorValues[0],
                 pressureSensorAdcValues[1],
                 pressureSensorValues[1]);
}

void PacketLogger::logFillingStationTelemetry(uint16_t* thermistorAdcValues,
                                              float* thermistorValues,
                                              uint16_t* pressureSensorAdcValues,
                                              float* pressureSensorValues,
                                              uint16_t* loadCellAdcValues,
                                              float* loadCellValues) {
    GCS_DATA_LOG(FILLING_STATION_TELEMETRY_LOG_TEMPLATE,
                 thermistorAdcValues[0],
                 thermistorValues[0],
                 thermistorAdcValues[1],
                 thermistorValues[1],
                 thermistorAdcValues[2],
                 thermistorValues[2],
                 thermistorAdcValues[3],
                 thermistorValues[3],
                 thermistorAdcValues[4],
                 thermistorValues[4],
                 thermistorAdcValues[5],
                 thermistorValues[5],
                 thermistorAdcValues[6],
                 thermistorValues[6],
                 thermistorAdcValues[7],
                 thermistorValues[7],
                 pressureSensorAdcValues[0],
                 pressureSensorValues[0],
                 pressureSensorAdcValues[1],
                 pressureSensorValues[1],
                 loadCellAdcValues[0],
                 loadCellValues[0],
                 loadCellAdcValues[1],
                 loadCellValues[1]);
}

void PacketLogger::logGsControl(GSControlStatus& status) {
    GCS_DATA_LOG(GS_CONTROL_PACKET_LOG_TEMPLATE,
                 onOff(status.bits.isAllowDumpSwitchOn),
                 onOff(status.bits.isAllowFillSwitchOn),
                 onOff(status.bits.isArmIgniterSwitchOn),
                 onOff(status.bits.isArmServoSwitchOn),
                 onOff(status.bits.isEmergencyStopButtonPressed),
                 onOff(status.bits.isFireIgniterButtonPressed),
                 onOff(status.bits.isUnsafeKeySwitchPressed),
                 onOff(status.bits.isValveStartButtonPressed));
}

void PacketLogger::logEngineStatus(ValveStatus& nosValveStatus, ValveStatus& ipaValveStatus) {
    GCS_DATA_LOG(ENGINE_STATUS_PACKET_LOG_TEMPLATE,
                 onOff(nosValveStatus.bits.isIdle),
                 onOff(nosValveStatus.bits.closedSwitchHigh),
                 onOff(nosValveStatus.bits.openedSwitchHigh),
                 onOff(ipaValveStatus.bits.isIdle),
                 onOff(ipaValveStatus.bits.closedSwitchHigh),
                 onOff(ipaValveStatus.bits.openedSwitchHigh));
}

void PacketLogger::logFillingStationStatus(ValveStatus& fillValveStatus, ValveStatus& dumpValveStatus) {
    GCS_DATA_LOG(FILLING_STATION_STATUS_PACKET_LOG_TEMPLATE,
                 onOff(fillValveStatus.bits.isIdle),
                 onOff(fillValveStatus.bits.closedSwitchHigh),
                 onOff(fillValveStatus.bits.openedSwitchHigh),
                 onOff(dumpValveStatus.bits.isIdle),
                 onOff(dumpValveStatus.bits.closedSwitchHigh),
                 onOff(dumpValveStatus.bits.openedSwitchHigh));
}

const char* PacketLogger::onOff(bool b) {
    return b ? "On" : "Off";
}
