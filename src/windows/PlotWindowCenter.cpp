#include "PlotWindowCenter.h"

#include "Constants.h"
#include "PlotDataCenter.h"

namespace PlotWindowCenter {
// clang-format off
PlotWindow accelerometerPlot        (Constants::GCS_ACCELEROMETER_PLOT_NAME, "X", "Y", 
                                     {&PlotDataCenter::AccelerometerXPlotData, &PlotDataCenter::AccelerometerYPlotData, &PlotDataCenter::AccelerometerZPlotData});
PlotWindow gyroscopePlot            (Constants::GCS_GYROSCOPE_PLOT_NAME, "X", "Y",
                                     {&PlotDataCenter::GyroscopeXPlotData, &PlotDataCenter::GyroscopeYPlotData, &PlotDataCenter::GyroscopeZPlotData});
PlotWindow altimeterPlot            (Constants::GCS_ALTIMETER_PLOT_NAME, "X", "Y",
                                     {&PlotDataCenter::AltimeterPlotData});
PlotWindow gpsPlot                  (Constants::GCS_GPS_PLOT_NAME, "X", "Y",
                                     {&PlotDataCenter::GpsLongitudePlotData, &PlotDataCenter::GpsLatitudePlotData});
PlotWindow magnetometerPlot         (Constants::GCS_MAGNETOMETER_PLOT_NAME, "X", "Y",
                                     {&PlotDataCenter::MagnetometerXPlotData, &PlotDataCenter::MagnetometerYPlotData, &PlotDataCenter::MagnetometerZPlotData});
PlotWindow thermistorPlot           ("Thermistors", "Timestamp (ms)", "Temperature (C)",
                                     {&PlotDataCenter::Thermistor1PlotData, &PlotDataCenter::Thermistor2PlotData, &PlotDataCenter::Thermistor3PlotData, &PlotDataCenter::Thermistor4PlotData, &PlotDataCenter::Thermistor5PlotData, &PlotDataCenter::Thermistor6PlotData, &PlotDataCenter::Thermistor7PlotData, &PlotDataCenter::Thermistor8PlotData});
PlotWindow pressureSensorPlot       ("Pressure Sensors", "Timestamp (ms)", "Pressure (psi)",
                                     {&PlotDataCenter::PressureSensor1PlotData, &PlotDataCenter::PressureSensor2PlotData});
PlotWindow loadCellPlot             ("Load Cells", "Timestamp (ms)", "Weight (lb)",
                                     {&PlotDataCenter::LoadCell1PlotData, &PlotDataCenter::LoadCell2PlotData});
// clang-format on
} // namespace PlotWindowCenter

std::vector<HelloImGui::DockableWindow> PlotWindowCenter::createDockableWindows() {
    // clang-format off
    HelloImGui::DockableWindow accelerometerPlotDockWin     (accelerometerPlot.getWindowId(),     Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::accelerometerPlot.render(); });
    HelloImGui::DockableWindow gyroscopePlotDockWin         (gyroscopePlot.getWindowId(),         Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::gyroscopePlot.render(); });
    HelloImGui::DockableWindow altimeterPlotDockWin         (altimeterPlot.getWindowId(),         Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::altimeterPlot.render(); });
    HelloImGui::DockableWindow gpsPlotDockWin               (gpsPlot.getWindowId(),               Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::gpsPlot.render(); });
    HelloImGui::DockableWindow magnetometerPlotDockWin      (magnetometerPlot.getWindowId(),      Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::magnetometerPlot.render(); });
    HelloImGui::DockableWindow thermistorPlotDockWin        (thermistorPlot.getWindowId(),        Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::thermistorPlot.render(); });
    HelloImGui::DockableWindow pressureSensorPlotDockWin    (pressureSensorPlot.getWindowId(),    Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::pressureSensorPlot.render(); });
    HelloImGui::DockableWindow loadCellPlotDockWin          (loadCellPlot.getWindowId(),          Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::loadCellPlot.render(); });
    // clang-format on

    return {accelerometerPlotDockWin, gyroscopePlotDockWin,      altimeterPlotDockWin,  gpsPlotDockWin,
            magnetometerPlotDockWin,  pressureSensorPlotDockWin, thermistorPlotDockWin, loadCellPlotDockWin};
}

void PlotWindowCenter::loadState(const mINI::INIStructure& iniStructure) {
    accelerometerPlot.loadState(iniStructure);
    gyroscopePlot.loadState(iniStructure);
    altimeterPlot.loadState(iniStructure);
    gpsPlot.loadState(iniStructure);
    magnetometerPlot.loadState(iniStructure);
    thermistorPlot.loadState(iniStructure);
    pressureSensorPlot.loadState(iniStructure);
    loadCellPlot.loadState(iniStructure);
}

void PlotWindowCenter::saveState(mINI::INIStructure& iniStructure) {
    accelerometerPlot.saveState(iniStructure);
    gyroscopePlot.saveState(iniStructure);
    altimeterPlot.saveState(iniStructure);
    gpsPlot.saveState(iniStructure);
    magnetometerPlot.saveState(iniStructure);
    thermistorPlot.saveState(iniStructure);
    pressureSensorPlot.saveState(iniStructure);
    loadCellPlot.saveState(iniStructure);
}
