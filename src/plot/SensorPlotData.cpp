#include "SensorPlotData.h"

/**
 * @brief Constructor for SensorPlotData.
 */
SensorPlotData::SensorPlotData(const char* name, ImVec4 color) : adcPlotData(name, color), valuePlotData(name, color) {
}

/**
 * @brief Add ADC and value data to the sensor plot.
 */
void SensorPlotData::addData(float adc, float value, float timestamp) {
    adcPlotData.addData(timestamp, adc);
    valuePlotData.addData(timestamp, value);
}

/**
 * @brief Plot the ADC data for the sensor.
 */
void SensorPlotData::plotAdc(bool showCompressedData) const {
    adcPlotData.plot(showCompressedData);
}

/**
 * @brief Plot the value data for the sensor.
 */
void SensorPlotData::plotValue(bool showCompressedData) const {
    valuePlotData.plot(showCompressedData);
}
