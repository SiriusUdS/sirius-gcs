#include "PacketProcessing.h"

#include "BoardComStateMonitor.h"
#include "GSDataCenter.h"
#include "LoadCell.h"
#include "Logging.h"
#include "PacketLogger.h"
#include "PacketRateMonitor.h"
#include "PacketReceiver.h"
#include "PressureTransducer.h"
#include "SensorPlotData.h"
#include "SerialCom.h"
#include "SerialConfig.h"
#include "SerialTask.h"
#include "SwitchData.h"
#include "Telecommunication/PacketHeaderVariable.h"
#include "Telecommunication/TelemetryPacket.h"
#include "TemperatureSensor.h"
#include "ValveData.h"

namespace PacketProcessing {
constexpr size_t THERMISTOR_ADC_VALUES_INDEX_OFFSET = 0;
constexpr size_t PRESSURE_SENSOR_INDEX_ADC_VALUES_INDEX_OFFSET = 8;
constexpr size_t PRESSURE_SENSOR_ADC_VALUES_INDEX_OFFSET = 10;
constexpr size_t LOAD_CELL_ADC_VALUES_INDEX_OFFSET = 14;

bool processIncomingPacket();
bool processEngineTelemetryPacket(PacketMetadata& metadata);
bool processFillingStationTelemetryPacket(PacketMetadata& metadata);
bool processGSControlPacket(PacketMetadata& metadata);
bool processEngineStatusPacket(PacketMetadata& metadata);
bool processFillingStationStatusPacket(PacketMetadata& metadata);
void computeThermistorValues(uint16_t thermistorAdcValues[GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD]);
void computePressureSensorValues(uint16_t pressureSensorIndices[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD],
                                 uint16_t pressureSensorAdcValues[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD]);
void computeLoadCellValues(uint16_t loadCellAdcValues[GSDataCenter::LOAD_CELL_AMOUNT]);
void addPlotData(SensorPlotData* plotData, uint16_t* adcValues, float* computedValues, size_t amount, float timestamp);
bool validateIncomingPacketSize(size_t currentPacketSize, size_t targetPacketSize, const char* packetName);

template <typename PacketType>
bool isPacketIntegrityValid(uint8_t* packetBuffer, PacketType packet, size_t sizeInBytes) {
    uint32_t computedCrc = CRC::computeCrc(packetBuffer, sizeInBytes - sizeof(packet->fields.crc));
    if (computedCrc != packet->fields.crc) {
        GCS_APP_LOG_WARN("PacketProcessing: CRC mismatch in {}, computed: {}, received: {}. Ignoring packet.",
                         typeid(*packet).name(),
                         computedCrc,
                         packet->fields.crc);
        return false;
    }
    return true;
}

uint8_t packetBuf[SerialConfig::MAX_PACKET_SIZE];
float thermistorValues[GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD]{};
float pressureSensorValues[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD]{};
float loadCellValues[GSDataCenter::LOAD_CELL_AMOUNT]{};
} // namespace PacketProcessing

void PacketProcessing::processIncomingPackets() {
    while (SerialTask::packetReceiver.packetAvailable()) {
        processIncomingPacket();
    }
}

bool PacketProcessing::processIncomingPacket() {
    std::optional<PacketMetadata> optionalPacketMetadata = SerialTask::packetReceiver.nextPacketMetadata();

    if (!optionalPacketMetadata.has_value()) {
        return false;
    }

    PacketMetadata packetMetadata = optionalPacketMetadata.value();

    if (!packetMetadata.isValid) {
        GCS_APP_LOG_DEBUG("PacketProcessing: Ignoring invalid packet.");
        SerialTask::packetReceiver.dumpNextPacket();
        return false;
    }

    if (!SerialTask::com.getPacket(packetBuf)) {
        GCS_APP_LOG_ERROR("PacketProcessing: Something went wrong while getting the next packet.");
        return false;
    }

    switch (packetMetadata.packetTypeCode) {
    case TELEMETRY_TYPE_CODE:
        if (packetMetadata.boardId == ENGINE_BOARD_ID) {
            return processEngineTelemetryPacket(packetMetadata);
        } else if (packetMetadata.boardId == FILLING_STATION_BOARD_ID) {
            return processFillingStationTelemetryPacket(packetMetadata);
        } else if (packetMetadata.boardId == GS_CONTROL_BOARD_ID) {
            GCS_APP_LOG_WARN("PacketProcessing: Tried processing GS control telemetry packet, but that doesn't exist.");
            return false;
        } else {
            GCS_APP_LOG_WARN("PacketProcessing: Telemetry packet contains invalid boardId, ignoring packet.");
            return false;
        }
    case STATUS_TYPE_CODE:
        if (packetMetadata.boardId == ENGINE_BOARD_ID) {
            return processEngineStatusPacket(packetMetadata);
        } else if (packetMetadata.boardId == FILLING_STATION_BOARD_ID) {
            return processFillingStationStatusPacket(packetMetadata);
        } else if (packetMetadata.boardId == GS_CONTROL_BOARD_ID) {
            return processGSControlPacket(packetMetadata);
        } else {
            GCS_APP_LOG_WARN("PacketProcessing: Status packet contains invalid boardId, ignoring packet.");
            return false;
        }
    }

    GCS_APP_LOG_ERROR("PacketProcessing: Unknown packet type. Ignoring packet.");
    return false;
}

bool PacketProcessing::processEngineTelemetryPacket(PacketMetadata& metadata) {
    if (!validateIncomingPacketSize(metadata.size, sizeof(EngineTelemetryPacket), "EngineTelemetryPacket")) {
        return false;
    }

    EngineTelemetryPacket* packet = reinterpret_cast<EngineTelemetryPacket*>(packetBuf);

    if (!isPacketIntegrityValid(packetBuf, packet, sizeof(EngineTelemetryPacket))) {
        return false;
    }

    float timestamp = static_cast<float>(packet->fields.timestamp_ms);
    uint16_t* adcValues = packet->fields.adcValues;

    uint16_t* thermistorAdcValues = adcValues + THERMISTOR_ADC_VALUES_INDEX_OFFSET;
    uint16_t* pressureSensorIndices = adcValues + PRESSURE_SENSOR_INDEX_ADC_VALUES_INDEX_OFFSET;
    uint16_t* pressureSensorAdcValues = adcValues + PRESSURE_SENSOR_ADC_VALUES_INDEX_OFFSET;

    computeThermistorValues(thermistorAdcValues);
    computePressureSensorValues(pressureSensorIndices, pressureSensorAdcValues);

    addPlotData(GSDataCenter::Thermistor_Motor_PlotData, thermistorAdcValues, thermistorValues, GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD, timestamp);
    addPlotData(GSDataCenter::PressureSensor_Motor_PlotData,
                pressureSensorAdcValues,
                pressureSensorValues,
                GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD,
                timestamp);

    SerialTask::packetRateMonitor.trackPacket();
    SerialTask::engineTelemetryPacketRateMonitor.trackPacket();
    SerialTask::motorBoardComStateMonitor.trackSuccessfulPacketRead();

    PacketLogger::logEngineTelemetry(thermistorAdcValues, thermistorValues, pressureSensorAdcValues, pressureSensorValues);
    return true;
}

bool PacketProcessing::processFillingStationTelemetryPacket(PacketMetadata& metadata) {
    if (!validateIncomingPacketSize(metadata.size, sizeof(FillingStationTelemetryPacket), "FillingStationTelemetryPacket")) {
        return false;
    }

    FillingStationTelemetryPacket* packet = reinterpret_cast<FillingStationTelemetryPacket*>(packetBuf);

    if (!isPacketIntegrityValid(packetBuf, packet, sizeof(FillingStationTelemetryPacket))) {
        return false;
    }

    float timestamp = static_cast<float>(packet->fields.timestamp_ms);
    uint16_t* adcValues = packet->fields.adcValues;

    uint16_t* thermistorAdcValues = adcValues + THERMISTOR_ADC_VALUES_INDEX_OFFSET;
    uint16_t* pressureSensorIndices = adcValues + PRESSURE_SENSOR_INDEX_ADC_VALUES_INDEX_OFFSET;
    uint16_t* pressureSensorAdcValues = adcValues + PRESSURE_SENSOR_ADC_VALUES_INDEX_OFFSET;
    uint16_t* loadCellAdcValues = adcValues + LOAD_CELL_ADC_VALUES_INDEX_OFFSET;

    computeThermistorValues(thermistorAdcValues);
    computePressureSensorValues(pressureSensorIndices, pressureSensorAdcValues);
    computeLoadCellValues(loadCellAdcValues);

    addPlotData(GSDataCenter::Thermistor_FillingStation_PlotData,
                thermistorAdcValues,
                thermistorValues,
                GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD,
                timestamp);
    addPlotData(GSDataCenter::PressureSensor_FillingStation_PlotData,
                pressureSensorAdcValues,
                pressureSensorValues,
                GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD,
                timestamp);
    addPlotData(GSDataCenter::LoadCell_FillingStation_PlotData, loadCellAdcValues, loadCellValues, GSDataCenter::LOAD_CELL_AMOUNT, timestamp);

    SerialTask::packetRateMonitor.trackPacket();
    SerialTask::fillingStationTelemetryPacketRateMonitor.trackPacket();
    SerialTask::fillingStationBoardComStateMonitor.trackSuccessfulPacketRead();

    PacketLogger::logFillingStationTelemetry(thermistorAdcValues,
                                             thermistorValues,
                                             pressureSensorAdcValues,
                                             pressureSensorValues,
                                             loadCellAdcValues,
                                             loadCellValues);
    return true;
}

bool PacketProcessing::processGSControlPacket(PacketMetadata& metadata) {
    if (!validateIncomingPacketSize(metadata.size, sizeof(GSControlStatusPacket), "GSControlStatusPacket")) {
        return false;
    }

    GSControlStatusPacket* packet = reinterpret_cast<GSControlStatusPacket*>(packetBuf);

    if (!isPacketIntegrityValid(packetBuf, packet, sizeof(GSControlStatusPacket))) {
        return false;
    }

    GSControlStatus& status = packet->fields.status;

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

    PacketLogger::logGsControl(status);
    return true;
}

bool PacketProcessing::processEngineStatusPacket(PacketMetadata& metadata) {
    if (!validateIncomingPacketSize(metadata.size, sizeof(EngineStatusPacket), "EngineStatusPacket")) {
        return false;
    }

    constexpr size_t NOS_VALVE_STATUS_INDEX = 0;
    constexpr size_t IPA_VALVE_STATUS_INDEX = 1;

    EngineStatusPacket* packet = reinterpret_cast<EngineStatusPacket*>(packetBuf);

    if (!isPacketIntegrityValid(packetBuf, packet, sizeof(EngineStatusPacket))) {
        return false;
    }

    ValveStatus& nosValveStatus = packet->fields.valveStatus[NOS_VALVE_STATUS_INDEX];
    ValveStatus& ipaValveStatus = packet->fields.valveStatus[IPA_VALVE_STATUS_INDEX];

    GSDataCenter::nosValveData.isIdle = nosValveStatus.bits.isIdle;
    GSDataCenter::nosValveData.closedSwitchHigh = nosValveStatus.bits.closedSwitchHigh;
    GSDataCenter::nosValveData.openedSwitchHigh = nosValveStatus.bits.openedSwitchHigh;

    GSDataCenter::ipaValveData.isIdle = ipaValveStatus.bits.isIdle;
    GSDataCenter::ipaValveData.closedSwitchHigh = ipaValveStatus.bits.closedSwitchHigh;
    GSDataCenter::ipaValveData.openedSwitchHigh = ipaValveStatus.bits.openedSwitchHigh;

    SerialTask::packetRateMonitor.trackPacket();
    SerialTask::engineStatusPacketRateMonitor.trackPacket();
    SerialTask::motorBoardComStateMonitor.trackSuccessfulPacketRead();

    PacketLogger::logEngineStatus(nosValveStatus, ipaValveStatus);
    return true;
}

bool PacketProcessing::processFillingStationStatusPacket(PacketMetadata& metadata) {
    if (!validateIncomingPacketSize(metadata.size, sizeof(FillingStationStatusPacket), "FillingStationStatusPacket")) {
        return false;
    }

    constexpr size_t FILL_VALVE_STATUS_INDEX = 0;
    constexpr size_t DUMP_VALVE_STATUS_INDEX = 1;

    FillingStationStatusPacket* packet = reinterpret_cast<FillingStationStatusPacket*>(packetBuf);

    if (!isPacketIntegrityValid(packetBuf, packet, sizeof(FillingStationStatusPacket))) {
        return false;
    }

    ValveStatus& fillValveStatus = packet->fields.valveStatus[FILL_VALVE_STATUS_INDEX];
    ValveStatus& dumpValveStatus = packet->fields.valveStatus[DUMP_VALVE_STATUS_INDEX];

    GSDataCenter::fillValveData.isIdle = fillValveStatus.bits.isIdle;
    GSDataCenter::fillValveData.closedSwitchHigh = fillValveStatus.bits.closedSwitchHigh;
    GSDataCenter::fillValveData.openedSwitchHigh = fillValveStatus.bits.openedSwitchHigh;

    GSDataCenter::dumpValveData.isIdle = dumpValveStatus.bits.isIdle;
    GSDataCenter::dumpValveData.closedSwitchHigh = dumpValveStatus.bits.closedSwitchHigh;
    GSDataCenter::dumpValveData.openedSwitchHigh = dumpValveStatus.bits.openedSwitchHigh;

    SerialTask::packetRateMonitor.trackPacket();
    SerialTask::fillingStationStatusPacketRateMonitor.trackPacket();
    SerialTask::fillingStationBoardComStateMonitor.trackSuccessfulPacketRead();

    PacketLogger::logFillingStationStatus(fillValveStatus, dumpValveStatus);
    return true;
}

void PacketProcessing::computeThermistorValues(uint16_t thermistorAdcValues[GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD]) {
    for (size_t i = 0; i < GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD; i++) {
        thermistorValues[i] = TemperatureSensor::adcToTemperature(thermistorAdcValues[i]);
    }
}

void PacketProcessing::computePressureSensorValues(uint16_t pressureSensorIndices[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD],
                                                   uint16_t pressureSensorAdcValues[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD]) {
    for (size_t i = 0; i < GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD; i++) {
        pressureSensorValues[i] = PressureTransducer::adcToPressure(pressureSensorAdcValues[i], pressureSensorIndices[i]);
    }
}

void PacketProcessing::computeLoadCellValues(uint16_t loadCellAdcValues[GSDataCenter::LOAD_CELL_AMOUNT]) {
    for (size_t i = 0; i < GSDataCenter::LOAD_CELL_AMOUNT; i++) {
        loadCellValues[i] = LoadCell::adcToForce(loadCellAdcValues[i], 0); // TODO: Change this index later?
    }
}

void PacketProcessing::addPlotData(SensorPlotData* plotData, uint16_t* adcValues, float* computedValues, size_t amount, float timestamp) {
    for (size_t i = 0; i < amount; i++) {
        plotData[i].addData(adcValues[i], computedValues[i], timestamp);
    }
}

bool PacketProcessing::validateIncomingPacketSize(size_t currentPacketSize, size_t targetPacketSize, const char* packetName) {
    if (currentPacketSize != targetPacketSize) {
        GCS_APP_LOG_WARN("PacketProcessing: Invalid {} size ({}), expected size ({}), ignoring packet.",
                         packetName,
                         currentPacketSize,
                         targetPacketSize);
        return false;
    }
    return true;
}
