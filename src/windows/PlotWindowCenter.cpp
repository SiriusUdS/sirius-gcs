#include "PlotWindowCenter.h"

#include "GSDataCenter.h"
#include "ImGuiConfig.h"

namespace PlotWindowCenter {
constexpr const char* TIMESTAMP_AXIS_NAME = "Timestamp (ms)";

PlotWindow thermistorPlot("Thermistors", TIMESTAMP_AXIS_NAME, "Temperature (C)",
                          {&GSDataCenter::ThermistorPlotData[0], &GSDataCenter::ThermistorPlotData[1], &GSDataCenter::ThermistorPlotData[2],
                           &GSDataCenter::ThermistorPlotData[3], &GSDataCenter::ThermistorPlotData[4], &GSDataCenter::ThermistorPlotData[5],
                           &GSDataCenter::ThermistorPlotData[6], &GSDataCenter::ThermistorPlotData[7]});
PlotWindow pressureSensorPlot("Pressure Sensors", TIMESTAMP_AXIS_NAME, "Pressure (psi)",
                              {&GSDataCenter::PressureSensorPlotData[0], &GSDataCenter::PressureSensorPlotData[1]});
PlotWindow loadCellPlot("Load Cells", TIMESTAMP_AXIS_NAME, "Weight (lb)", {&GSDataCenter::LoadCellPlotData[0], &GSDataCenter::LoadCellPlotData[1]});
} // namespace PlotWindowCenter

std::vector<HelloImGui::DockableWindow> PlotWindowCenter::createDockableWindows() {
    // clang-format off
    HelloImGui::DockableWindow thermistorPlotDockWin       (thermistorPlot.getWindowId(),     ImGuiConfig::Dockspace::PLOT, []() { PlotWindowCenter::thermistorPlot.render(); });
    HelloImGui::DockableWindow pressureSensorPlotDockWin   (pressureSensorPlot.getWindowId(), ImGuiConfig::Dockspace::PLOT, []() { PlotWindowCenter::pressureSensorPlot.render(); });
    HelloImGui::DockableWindow loadCellPlotDockWin         (loadCellPlot.getWindowId(),       ImGuiConfig::Dockspace::PLOT, []() { PlotWindowCenter::loadCellPlot.render(); });
    // clang-format on

    return {thermistorPlotDockWin, pressureSensorPlotDockWin, loadCellPlotDockWin};
}

void PlotWindowCenter::loadState(const mINI::INIStructure& iniStructure) {
    thermistorPlot.loadState(iniStructure);
    pressureSensorPlot.loadState(iniStructure);
    loadCellPlot.loadState(iniStructure);
}

void PlotWindowCenter::saveState(mINI::INIStructure& iniStructure) {
    thermistorPlot.saveState(iniStructure);
    pressureSensorPlot.saveState(iniStructure);
    loadCellPlot.saveState(iniStructure);
}
