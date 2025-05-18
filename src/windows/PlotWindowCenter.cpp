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
PlotWindow pressureSensorPlot       (Constants::GCS_PRESSURE_SENSOR_PLOT_NAME, "X", "Y",
                                     {&PlotDataCenter::PressureSensorPlotData});
PlotWindow temperatureSensorPlot    (Constants::GCS_TEMPERATURE_SENSOR_PLOT_NAME, "X", "Y",
                                     {&PlotDataCenter::TemperatureSensorPlotData});
PlotWindow loadCellPlot             ("Load Cells", "X", "Y",
                                     {&PlotDataCenter::LoadCell1Data, &PlotDataCenter::LoadCell2Data});
// clang-format on
// PlotWindow adcSensorPlot("ADC Values", "ms", "Value",
//                          {&PlotDataCenter::ADC1PlotData, &PlotDataCenter::ADC2PlotData, &PlotDataCenter::ADC3PlotData,
//                          &PlotDataCenter::ADC4PlotData,
//                           &PlotDataCenter::ADC5PlotData, &PlotDataCenter::ADC6PlotData, &PlotDataCenter::ADC7PlotData,
//                           &PlotDataCenter::ADC8PlotData, &PlotDataCenter::ADC9PlotData, &PlotDataCenter::ADC10PlotData,
//                           &PlotDataCenter::ADC11PlotData, &PlotDataCenter::ADC12PlotData, &PlotDataCenter::ADC13PlotData,
//                           &PlotDataCenter::ADC14PlotData, &PlotDataCenter::ADC15PlotData, &PlotDataCenter::ADC16PlotData});
PlotWindow adcSensorPlot("ADC Values", "ms", "Value", {&PlotDataCenter::ADC16PlotData});
} // namespace PlotWindowCenter

std::vector<HelloImGui::DockableWindow> PlotWindowCenter::createDockableWindows() {
    // clang-format off
    HelloImGui::DockableWindow accelerometerPlotDockWin     (accelerometerPlot.getWindowId(),     Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::accelerometerPlot.render(); });
    HelloImGui::DockableWindow gyroscopePlotDockWin         (gyroscopePlot.getWindowId(),         Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::gyroscopePlot.render(); });
    HelloImGui::DockableWindow altimeterPlotDockWin         (altimeterPlot.getWindowId(),         Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::altimeterPlot.render(); });
    HelloImGui::DockableWindow gpsPlotDockWin               (gpsPlot.getWindowId(),               Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::gpsPlot.render(); });
    HelloImGui::DockableWindow magnetometerPlotDockWin      (magnetometerPlot.getWindowId(),      Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::magnetometerPlot.render(); });
    HelloImGui::DockableWindow pressureSensorPlotDockWin    (pressureSensorPlot.getWindowId(),    Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::pressureSensorPlot.render(); });
    HelloImGui::DockableWindow temperatureSensorPlotDockWin (temperatureSensorPlot.getWindowId(), Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::temperatureSensorPlot.render(); });
    HelloImGui::DockableWindow loadCellPlotDockWin          (loadCellPlot.getWindowId(),          Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::loadCellPlot.render(); });
    HelloImGui::DockableWindow acdPlotDockWin               (adcSensorPlot.getWindowId(),         Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindowCenter::adcSensorPlot.render(); });
    // clang-format on

    return {accelerometerPlotDockWin,  gyroscopePlotDockWin,         altimeterPlotDockWin, gpsPlotDockWin, magnetometerPlotDockWin,
            pressureSensorPlotDockWin, temperatureSensorPlotDockWin, loadCellPlotDockWin,  acdPlotDockWin};
}

void PlotWindowCenter::loadState(const mINI::INIStructure& iniStructure) {
    accelerometerPlot.loadState(iniStructure);
    gyroscopePlot.loadState(iniStructure);
    altimeterPlot.loadState(iniStructure);
    gpsPlot.loadState(iniStructure);
    magnetometerPlot.loadState(iniStructure);
    pressureSensorPlot.loadState(iniStructure);
    temperatureSensorPlot.loadState(iniStructure);
    loadCellPlot.loadState(iniStructure);
    adcSensorPlot.loadState(iniStructure);
}

void PlotWindowCenter::saveState(mINI::INIStructure& iniStructure) {
    accelerometerPlot.saveState(iniStructure);
    gyroscopePlot.saveState(iniStructure);
    altimeterPlot.saveState(iniStructure);
    gpsPlot.saveState(iniStructure);
    magnetometerPlot.saveState(iniStructure);
    pressureSensorPlot.saveState(iniStructure);
    temperatureSensorPlot.saveState(iniStructure);
    loadCellPlot.saveState(iniStructure);
    adcSensorPlot.saveState(iniStructure);
}
