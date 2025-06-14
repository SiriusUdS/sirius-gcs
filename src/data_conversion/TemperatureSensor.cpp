#include "TemperatureSensor.h"

#include "Logging.h"

float TemperatureSensor::interpolateTemperature(float measuredResistance) {
    /**
     * @struct RT_Point
     * @brief Describes a point in the RT table
     */
    typedef struct {
        float TEMPERATURE; ///< Temperature in degrees celsius
        float RESISTANCE;  ///< Resistance in ohms
    } RT_Point;

    constexpr RT_Point RT_TABLE[] = {{-30, 1733200}, {-20, 959000}, {-10, 551410}, {0, 327240}, {10, 199990}, {20, 125250}, {25, 100000}, {30, 81000},
                                     {40, 53500},    {50, 35900},   {60, 25000},   {70, 17550}, {80, 12540},  {90, 9100},   {100, 6710}};
    constexpr int RT_TABLE_SIZE = sizeof(RT_TABLE) / sizeof(RT_TABLE[0]);

    for (int i = 0; i < RT_TABLE_SIZE - 1; i++) {
        if (measuredResistance <= RT_TABLE[i].RESISTANCE && measuredResistance >= RT_TABLE[i + 1].RESISTANCE) {
            float slope = (RT_TABLE[i + 1].TEMPERATURE - RT_TABLE[i].TEMPERATURE) / (RT_TABLE[i + 1].RESISTANCE - RT_TABLE[i].RESISTANCE);
            float temp = RT_TABLE[i].TEMPERATURE + slope * (measuredResistance - RT_TABLE[i].RESISTANCE);
            return temp;
        }
    }
    return -1;
}

float TemperatureSensor::convertToTemperature(float adcValue) {
    constexpr float ADC_MIN_TEMPERATURE = 10;
    constexpr float ADC_MAX_TEMPERATURE = 4090;
    constexpr int MAX_TEMPERATURE = 100;
    constexpr int MIN_TEMPERATURE = 0;
    constexpr float CONTROL_RESISTANCE = 10'000.0f;
    constexpr float ADDITIVE_FACTOR = 4096;

    // TEMP
    if (ADDITIVE_FACTOR == adcValue) {
        GCS_LOG_ERROR("THIS IS BAD");
        return -1;
    }
    float voltage = (adcValue / 4096.0f) * 3.3f;
    float resistance = (3.3f / voltage) * CONTROL_RESISTANCE;
    float measuredResistance = (CONTROL_RESISTANCE * adcValue) / (ADDITIVE_FACTOR - adcValue);
    return interpolateTemperature(resistance);
    // END

    float temperature = 0.0;

    if (adcValue < ADC_MIN_TEMPERATURE) {
        GCS_LOG_ERROR("Disconnected Temperature Sensor...Check connections or the device.");
        return -1;
    } else if (adcValue > ADC_MAX_TEMPERATURE) {
        GCS_LOG_ERROR("Short circuit detected on thermistor !");
        return -1;
    } else {
        float measuredResistance = (CONTROL_RESISTANCE * adcValue) / (ADDITIVE_FACTOR - adcValue); // Resistor value
        temperature = interpolateTemperature(measuredResistance);

        if (temperature < MIN_TEMPERATURE) {
            GCS_LOG_INFO("Too low temperature!");
            return -1;
        } else if (temperature > MAX_TEMPERATURE) {
            GCS_LOG_INFO("Too high temperature!");
            return -1;
        } else {
            GCS_LOG_INFO("Temperature: {}C", temperature);
        }
    }

    return temperature;
}
