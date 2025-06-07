#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "PlotData.h"

#include <implot.h>
#include <ini.h>
#include <vector>

/**
 * @class PlotWindow
 * @brief UI window to display plot data
 */
class PlotWindow {
public:
    PlotWindow(const char* name, const char* xLabel, const char* yLabel, std::vector<PlotData*> plotData);
    void render();
    void loadState(const mINI::INIStructure& ini);
    void saveState(mINI::INIStructure& ini);
    std::string getWindowId();

private:
    std::string name, xLabel, yLabel, autofitIniId, showCompressedDataIniId;
    std::vector<PlotData*> plotData;
    ImPlotFlags flags{};
    bool autofit{};
    bool showCompressedData{};
}; // namespace PlotWindow

#endif // PLOTWINDOW_H
