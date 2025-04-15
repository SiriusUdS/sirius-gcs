#include "PacketProcessing.h"

#include "Accelerometer/AccelerometerPacket.h"
#include "Altimeter/AltimeterPacket.h"
#include "Application.h"
#include "GPS/GpsPacket.h"
#include "Gyroscope/GyroscopePacket.h"
#include "Magnetometer/MagnetometerPacket.h"
#include "PlotDataCenter.h"
#include "PressureSensor/PressureSensorPacket.h"
#include "Rocket/RocketPacket.h"
#include "TemperatureSensor/TemperatureSensorPacket.h"
#include "Valve/ValvePacket.h"

bool PacketProcessing::processIncomingPacket() {
    uint32_t headerCode = Application::serialCom.nextPacketHeaderCode();

    switch (headerCode) {
    case 0:
        return false;
    case ACCELEROMETER_DATA_HEADER_CODE:
        return processAccelerometerPacket();
    case GYROSCOPE_DATA_HEADER_CODE:
        return processGyroscopePacket();
    case ALTIMETER_DATA_HEADER_CODE:
        return processAltimeterPacket();
    case GPS_DATA_HEADER_CODE:
        return processGpsPacket();
    case MAGNETOMETER_DATA_HEADER_CODE:
        return processMagnetometerPacket();
    case PRESSURE_SENSOR_DATA_HEADER_CODE:
        return processPressureSensorPacket();
    case ROCKET_DATA_HEADER_CODE:
        return processRocketPacket();
    case TEMPERATURE_SENSOR_DATA_HEADER_CODE:
        return processTemperatureSensorPacket();
    case VALVE_DATA_HEADER_CODE:
        return processValvePacket();
    }

    GCS_LOG_WARN("PacketProcessing: Unknown packet header code, ignoring packet.");
    return false;
}

bool PacketProcessing::processAccelerometerPacket() {
    size_t packetSize = Application::serialCom.nextPacketSize();
    if (packetSize != sizeof(AccelerometerPacket)) {
        if (!Application::serialCom.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: processAccelerometerPacket() called, but there's no packet to process.");
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid AccelerometerPacket size, ignoring packet.");
        return false;
    }

    AccelerometerPacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float rawX = packet.fields.rawData.members.data.rawX;
    float rawY = packet.fields.rawData.members.data.rawY;
    float rawZ = packet.fields.rawData.members.data.rawZ;
    PlotDataCenter::AccelerometerXPlotData.addData(timeStamp, rawX);
    PlotDataCenter::AccelerometerYPlotData.addData(timeStamp, rawY);
    PlotDataCenter::AccelerometerZPlotData.addData(timeStamp, rawZ);
    return true;
}

bool PacketProcessing::processGyroscopePacket() {
    size_t packetSize = Application::serialCom.nextPacketSize();
    if (packetSize != sizeof(GyroscopePacket)) {
        if (!Application::serialCom.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: processGyroscopePacket() called, but there's no packet to process.");
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid GyroscopePacket size, ignoring packet.");
        return false;
    }

    GyroscopePacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float rawX = packet.fields.rawData.members.data.rawX;
    float rawY = packet.fields.rawData.members.data.rawY;
    float rawZ = packet.fields.rawData.members.data.rawZ;
    PlotDataCenter::GyroscopeXPlotData.addData(timeStamp, rawX);
    PlotDataCenter::GyroscopeYPlotData.addData(timeStamp, rawY);
    PlotDataCenter::GyroscopeZPlotData.addData(timeStamp, rawZ);
    return true;
}

bool PacketProcessing::processAltimeterPacket() {
    size_t packetSize = Application::serialCom.nextPacketSize();
    if (packetSize != sizeof(AltimeterPacket)) {
        if (!Application::serialCom.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: processAltimeterPacket() called, but there's no packet to process.");
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid AltimeterPacket size, ignoring packet.");
        return false;
    }

    AltimeterPacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float rawAltitude = packet.fields.rawData.members.data.rawAltitude;
    PlotDataCenter::AltimeterPlotData.addData(timeStamp, rawAltitude);
    return true;
}

bool PacketProcessing::processGpsPacket() {
    size_t packetSize = Application::serialCom.nextPacketSize();
    if (packetSize != sizeof(GpsPacket)) {
        if (!Application::serialCom.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: processGpsPacket() called, but there's no packet to process.");
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid GpsPacket size, ignoring packet.");
        return false;
    }

    GpsPacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float rawLongitude = packet.fields.rawData.members.data.rawLongitude;
    float rawLatitude = packet.fields.rawData.members.data.rawLatitude;
    PlotDataCenter::GpsLongitudePlotData.addData(timeStamp, rawLongitude);
    PlotDataCenter::GpsLatitudePlotData.addData(timeStamp, rawLatitude);
    return true;
}

bool PacketProcessing::processMagnetometerPacket() {
    size_t packetSize = Application::serialCom.nextPacketSize();
    if (packetSize != sizeof(MagnetometerPacket)) {
        if (!Application::serialCom.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: processMagnetometerPacket() called, but there's no packet to process.");
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid MagnetometerPacket size, ignoring packet.");
        return false;
    }

    MagnetometerPacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float rawX = packet.fields.rawData.members.data.rawX;
    float rawY = packet.fields.rawData.members.data.rawY;
    float rawZ = packet.fields.rawData.members.data.rawZ;
    PlotDataCenter::MagnetometerXPlotData.addData(timeStamp, rawX);
    PlotDataCenter::MagnetometerYPlotData.addData(timeStamp, rawX);
    PlotDataCenter::MagnetometerZPlotData.addData(timeStamp, rawX);
    return true;
}

bool PacketProcessing::processPressureSensorPacket() {
    size_t packetSize = Application::serialCom.nextPacketSize();
    if (packetSize != sizeof(PressureSensorPacket)) {
        if (!Application::serialCom.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: processPressureSensorPacket() called, but there's no packet to process.");
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid PressureSensorPacket size, ignoring packet.");
        return false;
    }

    PressureSensorPacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float rawPressure = packet.fields.rawData.members.data.rawPressure;
    PlotDataCenter::PressureSensorPlotData.addData(timeStamp, rawPressure);
    return true;
}

bool PacketProcessing::processRocketPacket() {
    size_t packetSize = Application::serialCom.nextPacketSize();
    if (packetSize != sizeof(RocketPacket)) {
        if (!Application::serialCom.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: processRocketPacket() called, but there's no packet to process.");
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid RocketPacket size, ignoring packet.");
        return false;
    }

    RocketPacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.packet.rawData[0].members.timeStamp_ms; // TODO - changer quand yaura plus struct packet dans rocket packet
    PlotDataCenter::RocketPlotData.addData(timeStamp, 0);
    return true;
}

bool PacketProcessing::processTemperatureSensorPacket() {
    size_t packetSize = Application::serialCom.nextPacketSize();
    if (packetSize != sizeof(TemperatureSensorPacket)) {
        if (!Application::serialCom.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: processTemperatureSensorPacket() called, but there's no packet to process.");
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid TemperatureSensorPacket size, ignoring packet.");
        return false;
    }

    TemperatureSensorPacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float rawTemperature = packet.fields.rawData.members.data.rawTemperature;
    PlotDataCenter::TemperatureSensorPlotData.addData(timeStamp, rawTemperature);
    return true;
}

bool PacketProcessing::processValvePacket() {
    size_t packetSize = Application::serialCom.nextPacketSize();
    if (packetSize != sizeof(ValvePacket)) {
        if (!Application::serialCom.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: processValvePacket() called, but there's no packet to process.");
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid ValvePacket size, ignoring packet.");
        return false;
    }

    ValvePacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float status = packet.fields.rawData.members.status.bits.state;
    PlotDataCenter::ValvePlotData.addData(timeStamp, status);
    return true;
}
