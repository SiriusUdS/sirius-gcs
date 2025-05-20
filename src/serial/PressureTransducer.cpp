#include "PressureTransducer.h"

float voltageConverter_V(float adcValue) {
    float VoltageInput = (adcValue / PRESSURE_SENSOR_ADC_RANGE_BIT) * PRESSURE_SENSOR_VOLTAGE_RANGE_V;
    return VoltageInput;
}

float pressureConverter_NAME1_PSI(float VoltageInput, uint8_t index) {
    return (PRESSURE_SENSOR_ARRAY_SLOPE[index] * VoltageInput) + PRESSURE_SENSOR_ARRAY_CONST[index];
}