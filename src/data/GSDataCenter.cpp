#include "GSDataCenter.h"

#include "PlotColors.h"
#include "SensorPlotData.h"
#include "SwitchData.h"

namespace GSDataCenter {
SensorPlotData ThermistorPlotData[THERMISTOR_AMOUNT] = {{"Thermistor 1", PlotColors::BLUE},   {"Thermistor 2", PlotColors::RED},
                                                        {"Thermistor 3", PlotColors::GREEN},  {"Thermistor 4", PlotColors::YELLOW},
                                                        {"Thermistor 5", PlotColors::ORANGE}, {"Thermistor 6", PlotColors::CYAN},
                                                        {"Thermistor 7", PlotColors::PURPLE}, {"Thermistor 8", PlotColors::GRAY}};
SensorPlotData PressureSensorPlotData[PRESSURE_SENSOR_AMOUNT] = {{"Pressure Sensor 1", PlotColors::BLUE}, {"Pressure Sensor 2", PlotColors::RED}};
SensorPlotData LoadCellPlotData[LOAD_CELL_AMOUNT] = {{"Load Cell 1", PlotColors::BLUE}, {"Load Cell 2", PlotColors::RED}};

SwitchData AllowDumpSwitchData("Allow Dump", false);
SwitchData AllowFillSwitchData("Allow Fill", false);
SwitchData ArmIgniterSwitchData("Arm Igniter", false);
SwitchData ArmServoSwitchData("Arm Servo", false);
SwitchData EmergencyStopButtonData("Emergency Stop", false);
SwitchData FireIgniterButtonData("Fire Igniter", false);
SwitchData UnsafeKeySwitchData("Unsafe Key", false);
SwitchData ValveStartButtonData("Valve Start", false);
std::vector<SwitchData*> SwitchDataVec({&AllowDumpSwitchData, &AllowFillSwitchData, &ArmIgniterSwitchData, &ArmServoSwitchData,
                                        &EmergencyStopButtonData, &FireIgniterButtonData, &UnsafeKeySwitchData, &ValveStartButtonData});
} // namespace GSDataCenter
