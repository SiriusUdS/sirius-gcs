#include "TemperatureSensor.h"

#include "Logging.h"
#include "PlotConfig.h"

/**
 * @struct RT_Point
 * @brief Describes a point in the RT table
 */
typedef struct {
    float temperature; ///< Temperature in degrees celsius
    float resistance;  ///< Resistance in ohms
} RT_Point;

float TemperatureSensor::adcToTemperature(float adcValue) {
    static constexpr float MIN_VALID_ADC = 10;
    static constexpr float MAX_VALID_ADC = 4090;
    static constexpr float CONTROL_RESISTANCE = 10'000;
    static constexpr float ADDITIVE_FACTOR = 4096;
    static constexpr RT_Point RT_TABLE[] = {{-30, 1733200}, {-20, 959000}, {-10, 551410}, {0, 327240}, {10, 199990},
                                            {20, 125250},   {25, 100000},  {30, 81000},   {40, 53500}, {50, 35900},
                                            {60, 25000},    {70, 17550},   {80, 12540},   {90, 9100},  {100, 6710}};
    static constexpr int RT_TABLE_SIZE = sizeof(RT_TABLE) / sizeof(RT_TABLE[0]);

    // TODO: UNCOMMENT LATER
    // if (adcValue < MIN_VALID_ADC || adcValue > MAX_VALID_ADC) {
    //    GCS_LOG_DEBUG("TemperatureSensor: Invalid ADC value.");
    //    return PlotConfig::INVALID_VALUE;
    //}

    const float voltage = (adcValue / ADDITIVE_FACTOR) * 3.3f;
    const float resistance = (3.3f / voltage) * CONTROL_RESISTANCE;

    for (int i = 0; i < RT_TABLE_SIZE - 1; i++) {
        if (resistance <= RT_TABLE[i].resistance && resistance >= RT_TABLE[i + 1].resistance) {
            const float slope = (RT_TABLE[i + 1].temperature - RT_TABLE[i].temperature) / (RT_TABLE[i + 1].resistance - RT_TABLE[i].resistance);
            const float temp = RT_TABLE[i].temperature + slope * (resistance - RT_TABLE[i].resistance);
            return temp;
        }
    }

    // GCS_LOG_DEBUG("TemperatureSensor: Measured resistance not in RT table range.");
    // GCS_LOG_INFO("TemperatureSensor: Measured resistance not in RT table range.");
    // GCS_LOG_WARN("TemperatureSensor: Measured resistance not in RT table range.");
    // GCS_LOG_ERROR("TemperatureSensor: Measured resistance not in RT table range.");
    return PlotConfig::INVALID_VALUE;
}
