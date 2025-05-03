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
    if (!validateIncomingPacketSize(sizeof(AccelerometerPacket), "AccelerometerPacket")) {
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
    if (!validateIncomingPacketSize(sizeof(GyroscopePacket), "GyroscopePacket")) {
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
    if (!validateIncomingPacketSize(sizeof(AltimeterPacket), "AltimeterPacket")) {
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
    if (!validateIncomingPacketSize(sizeof(GpsPacket), "GpsPacket")) {
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
    if (!validateIncomingPacketSize(sizeof(MagnetometerPacket), "MagnetometerPacket")) {
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
    if (!validateIncomingPacketSize(sizeof(PressureSensorPacket), "PressureSensorPacket")) {
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
    if (!validateIncomingPacketSize(sizeof(RocketPacket), "RockerPacket")) {
        return false;
    }

    RocketPacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.packet.rawData[0].members.timeStamp_ms; // TODO - changer quand yaura plus struct packet dans rocket packet
    PlotDataCenter::RocketPlotData.addData(timeStamp, 0);
    return true;
}

float PacketProcessing::interpolateTemperature(float measuredResistance, RT_Point* rtTable, int tableSize) {
    for (int i = 0; i < tableSize - 1; i++) {
        if (measuredResistance <= rtTable[i].resistance && measuredResistance >= rtTable[i + 1].resistance) {
            float slope = (rtTable[i + 1].temperature - rtTable[i].temperature) / (rtTable[i + 1].resistance - rtTable[i].resistance);
            float temp = rtTable[i].temperature + slope * (measuredResistance - rtTable[i].resistance);
            return temp;
        }
    }
    return -1;
}

bool PacketProcessing::processTemperatureSensorPacket() {
    if (!validateIncomingPacketSize(sizeof(TemperatureSensorPacket), "TemperatureSensorPacket")) {
        return false;
    }

    TemperatureSensorPacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float rawTemperature = packet.fields.rawData.members.data.rawTemperature;
    float temperature = 0;

    if (rawTemperature < 10) {
        printf("ERREUR: Thermistance déconnectée !\n");
    } else if (rawTemperature > 4090) {
            // printf("ERREUR: Court-circuit détecté !\n");
    } else {
            // Calcul de la résistance de la thermistance
        float measuredResistance = (10000.0 * rawTemperature) / (4096.0 - rawTemperature);

        RT_Point rtTable[] = {{-30, 1733200}, {-20, 959000}, {-10, 551410}, {0, 327240}, {10, 199990}, {20, 125250}, {25, 100000}, {30, 81000},
                              {40, 53500},    {50, 35900},   {60, 25000},   {70, 17550}, {80, 12540},  {90, 9100},   {100, 6710}};

        int tableSize = sizeof(rtTable) / sizeof(rtTable[0]);
        temperature = interpolateTemperature(measuredResistance, rtTable, tableSize);

        if (temperature != -1) {
            printf("La température est de %.2f°C\n", temperature);
        } else {
            printf("Résistance mesurée en dehors de la plage du tableau.\n");
        }

        if (temperature < 0.0) {
            printf("ALERTE: Température trop basse: %.2f°C !\n", temperature);
        } else if (temperature > 100.0) {
            printf("ALERTE: Température trop élevée: %.2f°C !\n", temperature);
        } else if (temperature != -1) {
            printf("Température normale: %.2f°C\n", temperature);
        }

        //HAL_ADC_Stop(&hadc1);
    }
    PlotDataCenter::TemperatureSensorPlotData.addData(timeStamp, temperature);
    return true;
}

bool PacketProcessing::processValvePacket() {
    if (!validateIncomingPacketSize(sizeof(ValvePacket), "ValvePacket")) {
        return false;
    }

    ValvePacket packet;
    Application::serialCom.getPacket(packet.data);
    float timeStamp = packet.fields.rawData.members.timeStamp_ms;
    float status = packet.fields.rawData.members.status.bits.state;
    PlotDataCenter::ValvePlotData.addData(timeStamp, status);
    return true;
}

bool PacketProcessing::validateIncomingPacketSize(size_t targetPacketSize, const char* packetName) {
    size_t packetSize = Application::serialCom.nextPacketSize();
    if (packetSize != targetPacketSize) {
        if (!Application::serialCom.dumpNextPacket()) {
            GCS_LOG_WARN("PacketProcessing: process{}() called, but there's no packet to process.", packetName);
            return false;
        }
        GCS_LOG_WARN("PacketProcessing: Invalid {} size, ignoring packet.", packetName);
        return false;
    }
    return true;
}
