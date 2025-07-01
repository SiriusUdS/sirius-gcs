#include "PressureTransducer.h"

#include "Logging.h"
#include "PlotConfig.h"

float PressureTransducer::adcToPressure(float adcValue, uint16_t sensorIndex) {
    constexpr float PRESSURE_SENSOR_VOLTAGE_RANGE_V = 3.3f;
    constexpr float PRESSURE_SENSOR_ADC_RANGE_BIT = 4095.f;

    constexpr double PRESSURE_SENSOR_29673136_SLOPE = 1126.8605003120L;
    constexpr double PRESSURE_SENSOR_29673136_CONST = -32.4928073293L;
    constexpr double PRESSURE_SENSOR_29685373_SLOPE = 1158.2499813656L;
    constexpr double PRESSURE_SENSOR_29685373_CONST = 56.8114389484L;
    constexpr double PRESSURE_SENSOR_310989_SLOPE = 1164.9569321944L;
    constexpr double PRESSURE_SENSOR_310989_CONST = -79.7249315302L;
    constexpr double PRESSURE_SENSOR_3109815_SLOPE = 1160.5427492304L;
    constexpr double PRESSURE_SENSOR_3109815_CONST = -107.2803695064L;

    constexpr size_t PRESSURE_SENSOR_COUNT = 4;
    constexpr double PRESSURE_SENSOR_ARRAY_SLOPE[PRESSURE_SENSOR_COUNT] = {PRESSURE_SENSOR_29673136_SLOPE, PRESSURE_SENSOR_29685373_SLOPE,
                                                                           PRESSURE_SENSOR_310989_SLOPE, PRESSURE_SENSOR_3109815_SLOPE};
    constexpr double PRESSURE_SENSOR_ARRAY_CONST[PRESSURE_SENSOR_COUNT] = {PRESSURE_SENSOR_29673136_CONST, PRESSURE_SENSOR_29685373_CONST,
                                                                           PRESSURE_SENSOR_310989_CONST, PRESSURE_SENSOR_3109815_CONST};

    constexpr float ADC_MIN_PRESSURE = 0;
    constexpr float ADC_MAX_PRESSURE = 4095; // TODO: Remove if redundant

    if (sensorIndex >= PRESSURE_SENSOR_COUNT) {
        // TODO: UNCOMMENT LATER
        // GCS_LOG_ERROR("PressureTransducer: Sensor index out of range.");
        return PlotConfig::INVALID_VALUE;
    }

    // TODO: UNCOMMENT LATER
    // else if (adcValue < ADC_MIN_PRESSURE || adcValue > ADC_MAX_PRESSURE) {
    //    GCS_LOG_DEBUG("LoadCell:: ADC value out of range.");
    //    return PlotConfig::INVALID_VALUE;
    //}

    const float voltage = (adcValue / PRESSURE_SENSOR_ADC_RANGE_BIT) * PRESSURE_SENSOR_VOLTAGE_RANGE_V;
    return (float) ((PRESSURE_SENSOR_ARRAY_SLOPE[sensorIndex] * voltage) + PRESSURE_SENSOR_ARRAY_CONST[sensorIndex]);
}
