#ifndef PLOTWINDOWCENTER_H
#define PLOTWINDOWCENTER_H

#include "PlotWindow.h"

#include <hello_imgui/hello_imgui.h>

namespace PlotWindowCenter {
std::vector<HelloImGui::DockableWindow> createDockableWindows();
void loadState(const mINI::INIStructure& iniStructure);
void saveState(mINI::INIStructure& iniStructure);

extern PlotWindow accelerometerPlot;
extern PlotWindow gyroscopePlot;
extern PlotWindow altimeterPlot;
extern PlotWindow gpsPlot;
extern PlotWindow magnetometerPlot;
extern PlotWindow pressureSensorPlot;
extern PlotWindow temperatureSensorPlot;
extern PlotWindow loadCellPlot;
} // namespace PlotWindowCenter

#endif // PLOTWINDOWCENTER_H
