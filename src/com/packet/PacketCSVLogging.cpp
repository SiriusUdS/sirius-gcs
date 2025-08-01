#include "PacketCSVLogging.h"

#include "Logging.h"
#include "SerialConfig.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <string>

namespace PacketCSVLogging {
std::string getCurrentDateTimeForDir();

CSVLogger engineTelemetryLogger;
CSVLogger fillingStationTelemetryLogger;
CSVLogger gsControlLogger;
CSVLogger engineStatusLogger;
CSVLogger fillingStationStatusLogger;

bool dataLogDirExists{};
} // namespace PacketCSVLogging

void PacketCSVLogging::init() {
    std::string dataLogDir = "logs/data/" + getCurrentDateTimeForDir();

    try {
        std::filesystem::create_directories(dataLogDir);
        dataLogDirExists = true;
    } catch (const std::filesystem::filesystem_error& e) {
        GCS_APP_LOG_ERROR("PacketCSVLogging: Failed to create directory '{}': {}", dataLogDir, e.what());
        return;
    }

    engineTelemetryLogger.openFile(dataLogDir + "/EngineTelemetry.log");
    engineTelemetryLogger.addColumn("Timestamp");
    engineTelemetryLogger.addColumn("Thermistor 1");
    engineTelemetryLogger.addColumn("Thermistor 2");
    engineTelemetryLogger.addColumn("Thermistor 3");
    engineTelemetryLogger.addColumn("Thermistor 4");
    engineTelemetryLogger.addColumn("Thermistor 5");
    engineTelemetryLogger.addColumn("Thermistor 6");
    engineTelemetryLogger.addColumn("Thermistor 7");
    engineTelemetryLogger.addColumn("Thermistor 8");
    engineTelemetryLogger.addColumn("Pressure Sensor 1");
    engineTelemetryLogger.addColumn("Pressure Sensor 2");

    fillingStationTelemetryLogger.openFile(dataLogDir + "/FillingStationTelemetry.log");
    fillingStationTelemetryLogger.addColumn("Timestamp");
    fillingStationTelemetryLogger.addColumn("Thermistor 1");
    fillingStationTelemetryLogger.addColumn("Thermistor 2");
    fillingStationTelemetryLogger.addColumn("Thermistor 3");
    fillingStationTelemetryLogger.addColumn("Thermistor 4");
    fillingStationTelemetryLogger.addColumn("Thermistor 5");
    fillingStationTelemetryLogger.addColumn("Thermistor 6");
    fillingStationTelemetryLogger.addColumn("Thermistor 7");
    fillingStationTelemetryLogger.addColumn("Thermistor 8");
    fillingStationTelemetryLogger.addColumn("Pressure Sensor 1");
    fillingStationTelemetryLogger.addColumn("Pressure Sensor 2");
    fillingStationTelemetryLogger.addColumn("Motor Load Cell");
    fillingStationTelemetryLogger.addColumn("Tank Load Cell");

    gsControlLogger.openFile(dataLogDir + "/GSControl.log");
    gsControlLogger.addColumn("Timestamp");
    gsControlLogger.addColumn("Allow Dump");
    gsControlLogger.addColumn("Allow Fill");
    gsControlLogger.addColumn("Arm Igniter");
    gsControlLogger.addColumn("Arm Servo");
    gsControlLogger.addColumn("Emergency Stop");
    gsControlLogger.addColumn("Fire Igniter");
    gsControlLogger.addColumn("Valve Start");

    engineStatusLogger.openFile(dataLogDir + "/EngineStatus.log");
    engineStatusLogger.addColumn("Timestamp");
    engineStatusLogger.addColumn("NOS Valve Idle");
    engineStatusLogger.addColumn("NOS Valve Closed Switch High");
    engineStatusLogger.addColumn("NOS Valve Opened Switch High");
    engineStatusLogger.addColumn("IPA Valve Idle");
    engineStatusLogger.addColumn("IPA Valve Closed Switch High");
    engineStatusLogger.addColumn("IPA Valve Opened Switch High");

    fillingStationStatusLogger.openFile(dataLogDir + "/FillingStationStatus.log");
    fillingStationStatusLogger.addColumn("Timestamp");
    fillingStationStatusLogger.addColumn("Fill Valve Idle");
    fillingStationStatusLogger.addColumn("Fill Valve Closed Switch High");
    fillingStationStatusLogger.addColumn("Fill Valve Opened Switch High");
    fillingStationStatusLogger.addColumn("Dump Valve Idle");
    fillingStationStatusLogger.addColumn("Dump Valve Closed Switch High");
    fillingStationStatusLogger.addColumn("Dump Valve Opened Switch High");
}

void PacketCSVLogging::logEngineTelemetryPacket(float timestamp,
                                                float thermistorValues[GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD],
                                                float pressureSensorValues[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD]) {
    if (!dataLogDirExists) {
        return;
    }

    if (!engineTelemetryLogger.fileIsOpen()) {
        GCS_APP_LOG_WARN("PacketCSVLogging: Can't log engine telemetry packet, log file isn't open.");
        return;
    }

    engineTelemetryLogger.setValue(0, timestamp);

    size_t columnOffset = 1;
    for (size_t i = 0; i < GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD; i++) {
        engineTelemetryLogger.setValue(i + columnOffset, thermistorValues[i]);
    }

    columnOffset += GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD;
    for (size_t i = 0; i < GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD; i++) {
        engineTelemetryLogger.setValue(i + columnOffset, pressureSensorValues[i]);
    }

    engineTelemetryLogger.log();
}

void PacketCSVLogging::logFillingStationTelemetryPacket(float timestamp,
                                                        float thermistorValues[GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD],
                                                        float pressureSensorValues[GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD],
                                                        float loadCellValues[GSDataCenter::LOAD_CELL_AMOUNT]) {
    if (!dataLogDirExists) {
        return;
    }

    if (!fillingStationTelemetryLogger.fileIsOpen()) {
        GCS_APP_LOG_WARN("PacketCSVLogging: Can't log filling station telemetry packet, log file isn't open.");
        return;
    }

    fillingStationTelemetryLogger.setValue(0, timestamp);

    size_t columnOffset = 1;
    for (size_t i = 0; i < GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD; i++) {
        fillingStationTelemetryLogger.setValue(i + columnOffset, thermistorValues[i]);
    }

    columnOffset += GSDataCenter::THERMISTOR_AMOUNT_PER_BOARD;
    for (size_t i = 0; i < GSDataCenter::PRESSURE_SENSOR_AMOUNT_PER_BOARD; i++) {
        fillingStationTelemetryLogger.setValue(i + columnOffset, pressureSensorValues[i]);
    }

    columnOffset += GSDataCenter::LOAD_CELL_AMOUNT;
    for (size_t i = 0; i < GSDataCenter::LOAD_CELL_AMOUNT; i++) {
        fillingStationTelemetryLogger.setValue(i + columnOffset, loadCellValues[i]);
    }

    fillingStationTelemetryLogger.log();
}

void PacketCSVLogging::logGSControlPacket(const GSControlStatusPacket* packet) {
    if (!dataLogDirExists) {
        return;
    }

    if (!gsControlLogger.fileIsOpen()) {
        GCS_APP_LOG_WARN("PacketCSVLogging: Can't log GS control packet, log file isn't open.");
        return;
    }

    const GSControlStatus& status = packet->fields.status;

    gsControlLogger.setValue(0, static_cast<float>(packet->fields.timestamp_ms));
    gsControlLogger.setValue(1, static_cast<float>(status.bits.isAllowDumpSwitchOn));
    gsControlLogger.setValue(2, static_cast<float>(status.bits.isAllowFillSwitchOn));
    gsControlLogger.setValue(3, static_cast<float>(status.bits.isArmIgniterSwitchOn));
    gsControlLogger.setValue(4, static_cast<float>(status.bits.isArmServoSwitchOn));
    gsControlLogger.setValue(5, static_cast<float>(status.bits.isEmergencyStopButtonPressed));
    gsControlLogger.setValue(6, static_cast<float>(status.bits.isFireIgniterButtonPressed));
    gsControlLogger.setValue(7, static_cast<float>(status.bits.isValveStartButtonPressed));
    gsControlLogger.log();
}

void PacketCSVLogging::logEngineStatusPacket(const EngineStatusPacket* packet) {
    if (!dataLogDirExists) {
        return;
    }

    if (!engineStatusLogger.fileIsOpen()) {
        GCS_APP_LOG_WARN("PacketCSVLogging: Can't log engine status packet, log file isn't open.");
        return;
    }

    const ValveStatus& nosValve = packet->fields.valveStatus[SerialConfig::NOS_VALVE_STATUS_INDEX];
    const ValveStatus& ipaValve = packet->fields.valveStatus[SerialConfig::IPA_VALVE_STATUS_INDEX];

    engineStatusLogger.setValue(0, static_cast<float>(packet->fields.timestamp_ms));
    engineStatusLogger.setValue(1, static_cast<float>(nosValve.bits.isIdle));
    engineStatusLogger.setValue(2, static_cast<float>(nosValve.bits.closedSwitchHigh));
    engineStatusLogger.setValue(3, static_cast<float>(nosValve.bits.openedSwitchHigh));
    engineStatusLogger.setValue(4, static_cast<float>(ipaValve.bits.isIdle));
    engineStatusLogger.setValue(5, static_cast<float>(ipaValve.bits.closedSwitchHigh));
    engineStatusLogger.setValue(6, static_cast<float>(ipaValve.bits.openedSwitchHigh));
    engineStatusLogger.log();
}

void PacketCSVLogging::logFillingStationStatusPacket(const FillingStationStatusPacket* packet) {
    if (!dataLogDirExists) {
        return;
    }

    if (!fillingStationStatusLogger.fileIsOpen()) {
        GCS_APP_LOG_WARN("PacketCSVLogging: Can't log filling station status packet, log file isn't open.");
        return;
    }

    const ValveStatus& fillValve = packet->fields.valveStatus[SerialConfig::FILL_VALVE_STATUS_INDEX];
    const ValveStatus& dumpValve = packet->fields.valveStatus[SerialConfig::DUMP_VALVE_STATUS_INDEX];

    fillingStationStatusLogger.setValue(0, static_cast<float>(packet->fields.timestamp_ms));
    fillingStationStatusLogger.setValue(1, static_cast<float>(fillValve.bits.isIdle));
    fillingStationStatusLogger.setValue(2, static_cast<float>(fillValve.bits.closedSwitchHigh));
    fillingStationStatusLogger.setValue(3, static_cast<float>(fillValve.bits.openedSwitchHigh));
    fillingStationStatusLogger.setValue(4, static_cast<float>(dumpValve.bits.isIdle));
    fillingStationStatusLogger.setValue(5, static_cast<float>(dumpValve.bits.closedSwitchHigh));
    fillingStationStatusLogger.setValue(6, static_cast<float>(dumpValve.bits.openedSwitchHigh));
    fillingStationStatusLogger.log();
}

std::string PacketCSVLogging::getCurrentDateTimeForDir() {
    std::time_t now = std::time(nullptr);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &now);
#else
    localtime_r(&now, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}
