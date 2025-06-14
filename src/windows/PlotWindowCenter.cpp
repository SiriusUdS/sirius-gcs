#include "PlotWindowCenter.h"

#include "GSDataCenter.h"
#include "ImGuiConfig.h"

namespace PlotWindowCenter {
constexpr const char* TIMESTAMP_AXIS_NAME = "Timestamp (ms)";

PlotWindow thermistorPlot("Thermistors", TIMESTAMP_AXIS_NAME, "Temperature (C)",
                          {&GSDataCenter::Thermistor1PlotData, &GSDataCenter::Thermistor2PlotData, &GSDataCenter::Thermistor3PlotData,
                           &GSDataCenter::Thermistor4PlotData, &GSDataCenter::Thermistor5PlotData, &GSDataCenter::Thermistor6PlotData,
                           &GSDataCenter::Thermistor7PlotData, &GSDataCenter::Thermistor8PlotData});
PlotWindow pressureSensorPlot("Pressure Sensors", TIMESTAMP_AXIS_NAME, "Pressure (psi)",
                              {&GSDataCenter::PressureSensor1PlotData, &GSDataCenter::PressureSensor2PlotData});
PlotWindow loadCellPlot("Load Cells", TIMESTAMP_AXIS_NAME, "Weight (lb)", {&GSDataCenter::LoadCell1PlotData, &GSDataCenter::LoadCell2PlotData});
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
