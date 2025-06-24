#include "SensorPlotData.h"

SensorPlotData::SensorPlotData(const char* name, ImVec4 color) : adcPlotData(name, color), valuePlotData(name, color) {
}

void SensorPlotData::addData(float adc, float value, float timestamp) {
    adcPlotData.addData(adc, timestamp);
    valuePlotData.addData(value, timestamp);
}

void SensorPlotData::plotAdc(bool showCompressedData) const {
    adcPlotData.plot(showCompressedData);
}

void SensorPlotData::plotValue(bool showCompressedData) const {
    valuePlotData.plot(showCompressedData);
}
