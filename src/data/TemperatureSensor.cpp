#include "TemperatureSensor.h"

#include "Constants.h"
#include "Logging.h"

float TemperatureSensor::interpolateTemperature(float measuredResistance) {
    for (int i = 0; i < Constants::RT_TABLE_SIZE - 1; i++) {
        if (measuredResistance <= Constants::RT_TABLE[i].RESISTANCE && measuredResistance >= Constants::RT_TABLE[i + 1].RESISTANCE) {
            float slope = (Constants::RT_TABLE[i + 1].TEMPERATURE - Constants::RT_TABLE[i].TEMPERATURE)
                          / (Constants::RT_TABLE[i + 1].RESISTANCE - Constants::RT_TABLE[i].RESISTANCE);
            float temp = Constants::RT_TABLE[i].TEMPERATURE + slope * (measuredResistance - Constants::RT_TABLE[i].RESISTANCE);
            return temp;
        }
    }
    return -1;
}

float TemperatureSensor::convertToTemperature(float adcValue) {
    float temperature = 0.0;

    if (adcValue < Constants::ADC_MIN_TEMPERATURE) {
        GCS_LOG_ERROR("Disconnected Temperature Sensor...Check connections or the device.");
        return -1;
    } else if (adcValue > Constants::ADC_MAX_TEMPERATURE) {
        GCS_LOG_ERROR("Short circuit detected on thermistor !");
        return -1;
    } else {
        float measuredResistance = (Constants::MULTIPLICATIVE_FACTOR * adcValue) / (Constants::ADDITIVE_FACTOR - adcValue); // Resistor value
        temperature = interpolateTemperature(measuredResistance);

        if (temperature < Constants::MIN_TEMPERATURE) {
            GCS_LOG_INFO("Too low temperature!");
            return -1;
        } else if (temperature > Constants::MAX_TEMPERATURE) {
            GCS_LOG_INFO("Too high temperature!");
            return -1;
        } else {
            GCS_LOG_INFO("Temperature: {}C", temperature);
        }
    }

    return temperature;
}
