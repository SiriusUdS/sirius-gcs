#include "PressureTransducer.h"

#include "Logging.h"

float PressureTransducer::convertRawToPressure(float adcValue, uint8_t sensorIndex) {
    if (sensorIndex >= 4) {
        GCS_LOG_ERROR("PressureTransducer: damn");
        return 0;
    }

    double voltage = ((double) adcValue / PRESSURE_SENSOR_ADC_RANGE_BIT) * PRESSURE_SENSOR_VOLTAGE_RANGE_V;
    return (float) ((PRESSURE_SENSOR_ARRAY_SLOPE[sensorIndex] * voltage) + PRESSURE_SENSOR_ARRAY_CONST[sensorIndex]);
}
