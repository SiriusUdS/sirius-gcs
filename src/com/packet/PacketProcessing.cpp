#include "PacketProcessing.h"

#include "BoardComStateMonitor.h"
#include "GSDataCenter.h"
#include "LoadCell.h"
#include "Logging.h"
#include "PacketRateMonitor.h"
#include "PacketReceiver.h"
#include "PressureTransducer.h"
#include "SensorPlotData.h"
#include "SerialCom.h"
#include "SerialTask.h"
#include "SwitchData.h"
#include "Telecommunication/PacketHeaderVariable.h"
#include "Telecommunication/TelemetryPacket.h"
#include "TemperatureSensor.h"
#include "ValveData.h"

namespace PacketProcessing {
constexpr size_t MAX_PACKET_SIZE = 64;

bool processIncomingPacket();
bool processEngineTelemetryPacket();
bool processFillingStationTelemetryPacket();
bool processGSControlPacket();
bool processEngineStatusPacket();
bool processFillingStationStatusPacket();
void addThermistorPlotData(SensorPlotData plotData[GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD], uint16_t adcValues[ENGINE_ADC_CHANNEL_AMOUNT],
                           float timestamp);
void addPressureSensorPlotData(SensorPlotData plotData[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD], uint16_t adcValues[ENGINE_ADC_CHANNEL_AMOUNT],
                               float timestamp);
void addLoadCellPlotData(SensorPlotData plotData[GSDataCenter::LOAD_CELL_AMOUNT], uint16_t adcValues[ENGINE_ADC_CHANNEL_AMOUNT], float timestamp);
bool validateIncomingPacketSize(size_t targetPacketSize, const char* packetName);

size_t packetSize{};
uint8_t packetBuf[MAX_PACKET_SIZE];
std::string packetLog;
} // namespace PacketProcessing

void PacketProcessing::processIncomingPackets() {
    while (SerialTask::packetReceiver.nextPacketSize() > 0) {
        processIncomingPacket();
    }
}

bool PacketProcessing::processIncomingPacket() {
    packetSize = SerialTask::packetReceiver.nextPacketSize();

    if (packetSize == 0) {
        // No available packets
        return false;
    } else if (packetSize < sizeof(TelemetryHeader)) {
        GCS_APP_LOG_WARN("PacketProcessing: Received packet size ({}) too small to fit header ({}), ignoring packet.", packetSize,
                         sizeof(TelemetryHeader));
        SerialTask::packetReceiver.dumpNextPacket();
        return false;
    } else if (packetSize > MAX_PACKET_SIZE) {
        GCS_APP_LOG_WARN("PacketProcessing: Received packet size ({}) too big to fit in packet buffer ({}), ignoring packet", packetSize,
                         MAX_PACKET_SIZE);
        SerialTask::packetReceiver.dumpNextPacket();
        return false;
    }

    if (!SerialTask::com.getPacket(packetBuf)) {
        GCS_APP_LOG_ERROR("PacketProcessing: Something went wrong while getting the next packet.");
        return false;
    }

    TelemetryHeader* header = (TelemetryHeader*) packetBuf;

    switch (header->bits.type) {
    case TELEMETRY_TYPE_CODE:
        if (header->bits.boardId == ENGINE_BOARD_ID) {
            return processEngineTelemetryPacket();
        } else if (header->bits.boardId == FILLING_STATION_BOARD_ID) {
            return processFillingStationTelemetryPacket();
        } else if (header->bits.boardId == GS_CONTROL_BOARD_ID) {
            GCS_APP_LOG_WARN("PacketProcessing: Tried processing GS control telemetry packet, but that doesn't exist.");
            return false;
        } else {
            GCS_APP_LOG_WARN("PacketProcessing: Telemetry packet contains invalid boardId, ignoring packet.");
            return false;
        }
    case STATUS_TYPE_CODE:
        if (header->bits.boardId == ENGINE_BOARD_ID) {
            return processEngineStatusPacket();
        } else if (header->bits.boardId == FILLING_STATION_BOARD_ID) {
            return processFillingStationStatusPacket();
        } else if (header->bits.boardId == GS_CONTROL_BOARD_ID) {
            return processGSControlPacket();
        } else {
            GCS_APP_LOG_WARN("PacketProcessing: Status packet contains invalid boardId, ignoring packet.");
            return false;
        }
    }

    GCS_APP_LOG_WARN("PacketProcessing: Unknown packet type, ignoring packet.");
    return false;
}

bool PacketProcessing::processEngineTelemetryPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineTelemetryPacket), "EngineTelemetryPacket")) {
        return false;
    }

    EngineTelemetryPacket* packet = (EngineTelemetryPacket*) packetBuf;

    if (!isPacketIntegrityValid(packetBuf, packet, sizeof(EngineTelemetryPacket))) {
        return false;
    }

    float timestamp = (float) packet->fields.timestamp_ms;

    addThermistorPlotData(GSDataCenter::Thermistor_Motor_PlotData, packet->fields.adcValues, timestamp);
    addPressureSensorPlotData(GSDataCenter::PressureSensor_Motor_PlotData, packet->fields.adcValues, timestamp);

    SerialTask::packetRateMonitor.trackPacket();
    SerialTask::engineTelemetryPacketRateMonitor.trackPacket();
    SerialTask::motorBoardComStateMonitor.trackSuccessfulPacketRead();
    return true;
}

bool PacketProcessing::processFillingStationTelemetryPacket() {
    if (!validateIncomingPacketSize(sizeof(FillingStationTelemetryPacket), "FillingStationTelemetryPacket")) {
        return false;
    }

    FillingStationTelemetryPacket* packet = (FillingStationTelemetryPacket*) packetBuf;

    if (!isPacketIntegrityValid(packetBuf, packet, sizeof(FillingStationTelemetryPacket))) {
        return false;
    }

    float timestamp = (float) packet->fields.timestamp_ms;

    addThermistorPlotData(GSDataCenter::Thermistor_FillingStation_PlotData, packet->fields.adcValues, timestamp);
    addPressureSensorPlotData(GSDataCenter::PressureSensor_FillingStation_PlotData, packet->fields.adcValues, timestamp);
    addLoadCellPlotData(GSDataCenter::LoadCell_FillingStation_PlotData, packet->fields.adcValues, timestamp);

    SerialTask::packetRateMonitor.trackPacket();
    SerialTask::fillingStationTelemetryPacketRateMonitor.trackPacket();
    SerialTask::fillingStationBoardComStateMonitor.trackSuccessfulPacketRead();
    return true;
}

bool PacketProcessing::processGSControlPacket() {
    if (!validateIncomingPacketSize(sizeof(GSControlStatusPacket), "GSControlStatusPacket")) {
        return false;
    }

    GSControlStatusPacket* packet = (GSControlStatusPacket*) packetBuf;

    if (!isPacketIntegrityValid(packetBuf, packet, sizeof(GSControlStatusPacket))) {
        return false;
    }

    GSControlStatus status = packet->fields.status;

    GSDataCenter::AllowDumpSwitchData.isOn = status.bits.isAllowDumpSwitchOn;
    GSDataCenter::AllowFillSwitchData.isOn = status.bits.isAllowFillSwitchOn;
    GSDataCenter::ArmIgniterSwitchData.isOn = status.bits.isArmIgniterSwitchOn;
    GSDataCenter::ArmServoSwitchData.isOn = status.bits.isArmServoSwitchOn;
    GSDataCenter::EmergencyStopButtonData.isOn = status.bits.isEmergencyStopButtonPressed;
    GSDataCenter::FireIgniterButtonData.isOn = status.bits.isFireIgniterButtonPressed;
    GSDataCenter::UnsafeKeySwitchData.isOn = status.bits.isUnsafeKeySwitchPressed;
    GSDataCenter::ValveStartButtonData.isOn = status.bits.isValveStartButtonPressed;

    SerialTask::packetRateMonitor.trackPacket();
    SerialTask::gsControlPacketRateMonitor.trackPacket();
    SerialTask::gsControlBoardComStateMonitor.trackSuccessfulPacketRead();
    return true;
}

bool PacketProcessing::processEngineStatusPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineStatusPacket), "EngineStatusPacket")) {
        return false;
    }

    constexpr size_t NOS_VALVE_STATUS_INDEX = 0;
    constexpr size_t IPA_VALVE_STATUS_INDEX = 1;

    EngineStatusPacket* packet = (EngineStatusPacket*) packetBuf;

    if (!isPacketIntegrityValid(packetBuf, packet, sizeof(EngineStatusPacket))) {
        return false;
    }

    GSDataCenter::nosValveData.isIdle = packet->fields.valveStatus[NOS_VALVE_STATUS_INDEX].bits.isIdle;
    GSDataCenter::nosValveData.closedSwitchHigh = packet->fields.valveStatus[NOS_VALVE_STATUS_INDEX].bits.closedSwitchHigh;
    GSDataCenter::nosValveData.openedSwitchHigh = packet->fields.valveStatus[NOS_VALVE_STATUS_INDEX].bits.openedSwitchHigh;

    GSDataCenter::ipaValveData.isIdle = packet->fields.valveStatus[IPA_VALVE_STATUS_INDEX].bits.isIdle;
    GSDataCenter::ipaValveData.closedSwitchHigh = packet->fields.valveStatus[IPA_VALVE_STATUS_INDEX].bits.closedSwitchHigh;
    GSDataCenter::ipaValveData.openedSwitchHigh = packet->fields.valveStatus[IPA_VALVE_STATUS_INDEX].bits.openedSwitchHigh;

    SerialTask::packetRateMonitor.trackPacket();
    SerialTask::engineStatusPacketRateMonitor.trackPacket();
    SerialTask::motorBoardComStateMonitor.trackSuccessfulPacketRead();
    return true;
}

bool PacketProcessing::processFillingStationStatusPacket() {
    if (!validateIncomingPacketSize(sizeof(FillingStationStatusPacket), "FillingStationStatusPacket")) {
        return false;
    }

    constexpr size_t FILL_VALVE_STATUS_INDEX = 0;
    constexpr size_t DUMP_VALVE_STATUS_INDEX = 1;

    FillingStationStatusPacket* packet = (FillingStationStatusPacket*) packetBuf;

    if (!isPacketIntegrityValid(packetBuf, packet, sizeof(FillingStationStatusPacket))) {
        return false;
    }

    GSDataCenter::fillValveData.isIdle = packet->fields.valveStatus[FILL_VALVE_STATUS_INDEX].bits.isIdle;
    GSDataCenter::fillValveData.closedSwitchHigh = packet->fields.valveStatus[FILL_VALVE_STATUS_INDEX].bits.closedSwitchHigh;
    GSDataCenter::fillValveData.openedSwitchHigh = packet->fields.valveStatus[FILL_VALVE_STATUS_INDEX].bits.openedSwitchHigh;

    GSDataCenter::dumpValveData.isIdle = packet->fields.valveStatus[DUMP_VALVE_STATUS_INDEX].bits.isIdle;
    GSDataCenter::dumpValveData.closedSwitchHigh = packet->fields.valveStatus[DUMP_VALVE_STATUS_INDEX].bits.closedSwitchHigh;
    GSDataCenter::dumpValveData.openedSwitchHigh = packet->fields.valveStatus[DUMP_VALVE_STATUS_INDEX].bits.openedSwitchHigh;

    SerialTask::packetRateMonitor.trackPacket();
    SerialTask::fillingStationStatusPacketRateMonitor.trackPacket();
    SerialTask::fillingStationBoardComStateMonitor.trackSuccessfulPacketRead();
    return true;
}

void PacketProcessing::addThermistorPlotData(SensorPlotData plotData[GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD],
                                             uint16_t adcValues[ENGINE_ADC_CHANNEL_AMOUNT], float timestamp) {
    static constexpr size_t THERMISTOR_ADC_INDEX_OFFSET = 0;

    for (size_t i = 0; i < GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD; i++) {
        const size_t adcIdx = i + THERMISTOR_ADC_INDEX_OFFSET;
        const float adcValue = adcValues[adcIdx];
        const float temperature = TemperatureSensor::adcToTemperature(adcValue);
        plotData[i].addData(adcValue, temperature, timestamp);
    }
}

void PacketProcessing::addPressureSensorPlotData(SensorPlotData plotData[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD],
                                                 uint16_t adcValues[ENGINE_ADC_CHANNEL_AMOUNT], float timestamp) {
    static constexpr size_t PRESSURE_SENSOR_INDEX_ADC_INDEX_OFFSET = 8;
    static constexpr size_t PRESSURE_SENSOR_ADC_INDEX_OFFSET = 10;

    for (size_t i = 0; i < GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD; i++) {
        const size_t sensorIdxAdcIdx = i + PRESSURE_SENSOR_INDEX_ADC_INDEX_OFFSET;
        const uint16_t sensorIdx = adcValues[sensorIdxAdcIdx];
        const size_t pressureSensorAdcIdx = i + PRESSURE_SENSOR_ADC_INDEX_OFFSET;
        const float adcValue = adcValues[pressureSensorAdcIdx];
        const float pressure = PressureTransducer::adcToPressure(adcValue, sensorIdx);
        plotData[i].addData(adcValue, pressure, timestamp);
    }
}

void PacketProcessing::addLoadCellPlotData(SensorPlotData plotData[GSDataCenter::LOAD_CELL_AMOUNT], uint16_t adcValues[ENGINE_ADC_CHANNEL_AMOUNT],
                                           float timestamp) {
    static constexpr size_t LOAD_CELL_ADC_INDEX_OFFSET = 14;

    for (size_t i = 0; i < GSDataCenter::LOAD_CELL_AMOUNT; i++) {
        const size_t adcIdx = i + LOAD_CELL_ADC_INDEX_OFFSET;
        const float adcValue = adcValues[adcIdx];
        const float force = LoadCell::adcToForce((float) adcValues[adcIdx], 0);
        plotData[i].addData(adcValue, force, timestamp);
    }
}

bool PacketProcessing::validateIncomingPacketSize(size_t targetPacketSize, const char* packetName) {
    if (packetSize != targetPacketSize) {
        GCS_APP_LOG_WARN("PacketProcessing: Invalid {} size, ignoring packet.", packetName);
        return false;
    }
    return true;
}
