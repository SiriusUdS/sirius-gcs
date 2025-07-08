#include "PacketProcessing.h"

#include "GSDataCenter.h"
#include "LoadCell.h"
#include "Logging.h"
#include "PacketReceiver.h"
#include "PressureTransducer.h"
#include "SensorPlotData.h"
#include "SerialCom.h"
#include "SerialTask.h"
#include "SwitchData.h"
#include "Telecommunication/PacketHeaderVariable.h"
#include "Telecommunication/TelemetryPacket.h"
#include "TemperatureSensor.h"

namespace PacketProcessing {
constexpr size_t MAX_PACKET_SIZE = 64;

bool processIncomingPacket();
bool processEngineTelemetryPacket();
bool processGSControlPacket();
bool processEngineStatusPacket();
bool validateIncomingPacketSize(size_t targetPacketSize, const char* packetName);

size_t packetSize{};
uint8_t packetBuf[MAX_PACKET_SIZE];
} // namespace PacketProcessing

void PacketProcessing::processIncomingPackets() {
    while (processIncomingPacket()) {
    }
}

bool PacketProcessing::processIncomingPacket() {
    packetSize = SerialTask::packetReceiver.nextPacketSize();

    if (packetSize == 0) {
        // No available packets
        return false;
    } else if (packetSize < sizeof(TelemetryHeader)) {
        GCS_LOG_WARN("PacketProcessing: Received packet size ({}) too small to fit header ({}), ignoring packet.", packetSize,
                     sizeof(TelemetryHeader));
        SerialTask::packetReceiver.dumpNextPacket();
        return false;
    } else if (packetSize > MAX_PACKET_SIZE) {
        GCS_LOG_WARN("PacketProcessing: Received packet size ({}) too big to fit in packet buffer ({}), ignoring packet", packetSize,
                     MAX_PACKET_SIZE);
        SerialTask::packetReceiver.dumpNextPacket();
        return false;
    }

    if (!SerialTask::com.getPacket(packetBuf)) {
        GCS_LOG_ERROR("PacketProcessing: Something went wrong while getting the next packet.");
        return false;
    }

    TelemetryHeader* header = (TelemetryHeader*) packetBuf;
    switch (header->bits.type) {
    case TELEMETRY_TYPE_CODE:
        return processEngineTelemetryPacket();
    case STATUS_TYPE_CODE:
        if (header->bits.boardId == GS_CONTROL_BOARD_ID) {
            return processGSControlPacket();
        } else if (header->bits.boardId == ENGINE_BOARD_ID) {
            return processEngineStatusPacket();
        } else {
            GCS_LOG_WARN("PacketProcessing: Status packet contains invalid boardId, ignoring packet.");
            return false;
        }
    }

    GCS_LOG_WARN("PacketProcessing: Unknown packet type, ignoring packet.");
    return false;
}

bool PacketProcessing::processEngineTelemetryPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineTelemetryPacket), "EngineTelemetryPacket")) {
        return false;
    }

    EngineTelemetryPacket* packet = (EngineTelemetryPacket*) packetBuf;
    float timestamp = (float) packet->fields.timestamp_ms;

    static constexpr size_t THERMISTOR_ADC_INDEX_OFFSET = 0;
    static constexpr size_t PRESSURE_SENSOR_INDEX_ADC_INDEX_OFFSET = 8;
    static constexpr size_t PRESSURE_SENSOR_ADC_INDEX_OFFSET = 10;
    static constexpr size_t LOAD_CELL_ADC_INDEX_OFFSET = 14;

    // Thermistors
    for (size_t i = 0; i < GSDataCenter::THERMISTOR_AMOUNT; i++) {
        const size_t adcIdx = i + THERMISTOR_ADC_INDEX_OFFSET;
        const float adcValue = packet->fields.adcValues[adcIdx];
        const float temperature = TemperatureSensor::adcToTemperature(packet->fields.adcValues[adcIdx]);

        GSDataCenter::ThermistorPlotData[i].addData(adcValue, temperature, timestamp);
    }

    // Pressure sensors
    for (size_t i = 0; i < GSDataCenter::PRESSURE_SENSOR_AMOUNT; i++) {
        const size_t sensorIdxAdcIdx = i + PRESSURE_SENSOR_INDEX_ADC_INDEX_OFFSET;
        const uint16_t sensorIdx = packet->fields.adcValues[sensorIdxAdcIdx];
        const size_t pressureSensorAdcIdx = i + PRESSURE_SENSOR_ADC_INDEX_OFFSET;
        const float adcValue = packet->fields.adcValues[pressureSensorAdcIdx];
        const float pressure = PressureTransducer::adcToPressure(packet->fields.adcValues[pressureSensorAdcIdx], sensorIdx);

        GSDataCenter::PressureSensorPlotData[i].addData(adcValue, pressure, timestamp);
    }

    // Load cells
    for (size_t i = 0; i < GSDataCenter::LOAD_CELL_AMOUNT; i++) {
        const size_t adcIdx = i + LOAD_CELL_ADC_INDEX_OFFSET;
        const float adcValue = packet->fields.adcValues[adcIdx];
        const float force = LoadCell::adcToForce((float) packet->fields.adcValues[adcIdx], 0);

        GSDataCenter::LoadCellPlotData[i].addData(adcValue, force, timestamp);
    }

    return true;
}

bool PacketProcessing::processGSControlPacket() {
    if (!validateIncomingPacketSize(sizeof(GSControlStatusPacket), "GSControlStatusPacket")) {
        return false;
    }

    GSControlStatusPacket* packet = (GSControlStatusPacket*) packetBuf;

    GSDataCenter::AllowDumpSwitchData.isOn = packet->fields.status.bits.isAllowDumpSwitchOn;
    GSDataCenter::AllowFillSwitchData.isOn = packet->fields.status.bits.isAllowFillSwitchOn;
    GSDataCenter::ArmIgniterSwitchData.isOn = packet->fields.status.bits.isArmIgniterSwitchOn;
    GSDataCenter::ArmServoSwitchData.isOn = packet->fields.status.bits.isArmServoSwitchOn;
    GSDataCenter::EmergencyStopButtonData.isOn = packet->fields.status.bits.isEmergencyStopButtonPressed;
    GSDataCenter::FireIgniterButtonData.isOn = packet->fields.status.bits.isFireIgniterButtonPressed;
    GSDataCenter::UnsafeKeySwitchData.isOn = packet->fields.status.bits.isUnsafeKeySwitchPressed;
    GSDataCenter::ValveStartButtonData.isOn = packet->fields.status.bits.isValveStartButtonPressed;

    return true;
}

bool PacketProcessing::processEngineStatusPacket() {
    if (!validateIncomingPacketSize(sizeof(EngineStatusPacket), "EngineStatusPacket")) {
        return false;
    }

    EngineStatusPacket* packet = (EngineStatusPacket*) packetBuf;
    return true;
}

bool PacketProcessing::validateIncomingPacketSize(size_t targetPacketSize, const char* packetName) {
    if (packetSize != targetPacketSize) {
        GCS_LOG_WARN("PacketProcessing: Invalid {} size, ignoring packet.", packetName);
        return false;
    }
    return true;
}
