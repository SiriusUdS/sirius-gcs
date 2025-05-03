#include "TemperatureSensor.h"

#include "../Constants.h"
#include "logging/Logging.h"

float TemperatureSensor::interpolateTemperature(float measuredResistance) {
    for (int i = 0; i < Constants::tableSize - 1; i++) {
        if (measuredResistance <= Constants::rtTable[i].resistance && measuredResistance >= Constants::rtTable[i + 1].resistance) {
            float slope = (Constants::rtTable[i + 1].temperature - Constants::rtTable[i].temperature)
                          / (Constants::rtTable[i + 1].resistance - Constants::rtTable[i].resistance);
            float temp = Constants::rtTable[i].temperature + slope * (measuredResistance - Constants::rtTable[i].resistance);
            return temp;
        }
    }
    return -1;
}

float TemperatureSensor::temperatureInfos(float adc_value) {
    float temperature = 0.0;

    if (adc_value < Constants::ADC_MIN_TEMPERATURE) {
        GCS_LOG_ERROR("Disconnected Temperature Sensor...Check connections or the device.\n");
    } else if (adc_value > Constants::ADC_MAX_TEMPERATURE) {
        GCS_LOG_ERROR("Short circuit detected on thermistor !\n");
    } else {
        float measuredResistance = (10000.0 * adc_value) / (4096.0 - adc_value); // Resistor value
        temperature = interpolateTemperature(measuredResistance);

        if (temperature < 0.0) {
            GCS_LOG_INFO("Too low temperature: {}°C !\n", temperature);
        } else if (temperature > 100.0) {
            GCS_LOG_INFO("Too high temperature: {}°C !\n", temperature);
        } else {
            GCS_LOG_INFO("Temperature: {}°C\n", temperature);
        }
    }

    return temperature;
}