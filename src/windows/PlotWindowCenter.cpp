#include "PlotWindowCenter.h"

#include "Constants.h"
#include "GSDataCenter.h"

namespace PlotWindowCenter {
// clang-format off
PlotWindow thermistorPlot           ("Thermistors", Constants::PLOT_AXIS_TIMESTAMP_NAME, "Temperature (C)",
                                     {&GSDataCenter::Thermistor1PlotData, &GSDataCenter::Thermistor2PlotData, &GSDataCenter::Thermistor3PlotData, &GSDataCenter::Thermistor4PlotData, &GSDataCenter::Thermistor5PlotData, &GSDataCenter::Thermistor6PlotData, &GSDataCenter::Thermistor7PlotData, &GSDataCenter::Thermistor8PlotData});
PlotWindow pressureSensorPlot       ("Pressure Sensors", Constants::PLOT_AXIS_TIMESTAMP_NAME, "Pressure (psi)",
                                     {&GSDataCenter::PressureSensor1PlotData, &GSDataCenter::PressureSensor2PlotData});
PlotWindow loadCellPlot             ("Load Cells", Constants::PLOT_AXIS_TIMESTAMP_NAME, "Weight (lb)",
                                     {&GSDataCenter::LoadCell1PlotData, &GSDataCenter::LoadCell2PlotData});
// clang-format on
} // namespace PlotWindowCenter

std::vector<HelloImGui::DockableWindow> PlotWindowCenter::createDockableWindows() {
    // clang-format off
    HelloImGui::DockableWindow thermistorPlotDockWin        (thermistorPlot.getWindowId(),        Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::thermistorPlot.render(); });
    HelloImGui::DockableWindow pressureSensorPlotDockWin    (pressureSensorPlot.getWindowId(),    Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::pressureSensorPlot.render(); });
    HelloImGui::DockableWindow loadCellPlotDockWin          (loadCellPlot.getWindowId(),          Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::loadCellPlot.render(); });
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
