#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <implot.h>
#include <ini.h>
#include <vector>

class SensorPlotData;

/**
 * @class PlotWindow
 * @brief UI window to display plot data
 */
class PlotWindow {
public:
    PlotWindow(const char* name, const char* xLabel, const char* yLabel, std::vector<SensorPlotData*> plotData);
    void render();
    void loadState(const mINI::INIStructure& ini);
    void saveState(mINI::INIStructure& ini);
    std::string getWindowId();

private:
    enum DataType { VALUE = 0, ADC = 1 };

    std::string name, xLabel, yLabel, autofitIniId, showCompressedDataIniId, dataTypeIniId;
    std::vector<SensorPlotData*> plotData;
    ImPlotFlags flags{};
    bool autofit{};
    bool showCompressedData{};
    int dataType{};
}; // namespace PlotWindow

#endif // PLOTWINDOW_H
