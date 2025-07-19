#include "GSDataCenter.h"

#include "Engine/EngineSensors.h"
#include "FillingStation/FillingStationSensors.h"
#include "PlotColors.h"
#include "SensorPlotData.h"
#include "SwitchData.h"
#include "ValveData.h"

namespace GSDataCenter {
SensorPlotData Thermistor_Motor_PlotData[THERMISTOR_AMOUNT_PER_BOARD] = {{"Thermistor 1", PlotColors::BLUE},
                                                                         {"Thermistor 2", PlotColors::RED},
                                                                         {"Thermistor 3", PlotColors::GREEN},
                                                                         {"Thermistor 4", PlotColors::YELLOW},
                                                                         {"Thermistor 5", PlotColors::ORANGE},
                                                                         {"Thermistor 6", PlotColors::CYAN},
                                                                         {"Thermistor 7", PlotColors::PURPLE},
                                                                         {"Thermistor 8", PlotColors::GRAY}};
SensorPlotData PressureSensor_Motor_PlotData[PRESSURE_SENSOR_AMOUNT_PER_BOARD] = {
  {"Pressure Sensor 1", PlotColors::BLUE},
  {"Pressure Sensor 2", PlotColors::RED}};
SensorPlotData Thermistor_FillingStation_PlotData[THERMISTOR_AMOUNT_PER_BOARD] = {{"Thermistor 1", PlotColors::BLUE},
                                                                                  {"Thermistor 2", PlotColors::RED},
                                                                                  {"Thermistor 3", PlotColors::GREEN},
                                                                                  {"Thermistor 4", PlotColors::YELLOW},
                                                                                  {"Thermistor 5", PlotColors::ORANGE},
                                                                                  {"Thermistor 6", PlotColors::CYAN},
                                                                                  {"Thermistor 7", PlotColors::PURPLE},
                                                                                  {"Thermistor 8", PlotColors::GRAY}};
SensorPlotData PressureSensor_FillingStation_PlotData[PRESSURE_SENSOR_AMOUNT_PER_BOARD] = {
  {"Pressure Sensor 1", PlotColors::BLUE},
  {"Pressure Sensor 2", PlotColors::RED}};
SensorPlotData LoadCell_FillingStation_PlotData[LOAD_CELL_AMOUNT] = {{"Motor Load Cell", PlotColors::BLUE},
                                                                     {"Tank Load Cell", PlotColors::RED}};

SwitchData AllowDumpSwitchData{.name = "Allow Dump"};
SwitchData AllowFillSwitchData{.name = "Allow Fill"};
SwitchData ArmIgniterSwitchData{.name = "Arm Igniter"};
SwitchData ArmServoSwitchData{.name = "Arm Servo"};
SwitchData EmergencyStopButtonData{.name = "Emergency Stop"};
SwitchData FireIgniterButtonData{.name = "Fire Igniter"};
SwitchData UnsafeKeySwitchData{.name = "Unsafe Key"};
SwitchData ValveStartButtonData{.name = "Valve Start"};
std::vector<SwitchData*> SwitchDataVec({&AllowDumpSwitchData,
                                        &AllowFillSwitchData,
                                        &ArmIgniterSwitchData,
                                        &ArmServoSwitchData,
                                        &EmergencyStopButtonData,
                                        &FireIgniterButtonData,
                                        &UnsafeKeySwitchData,
                                        &ValveStartButtonData});

ValveData nosValveData;
ValveData ipaValveData;
ValveData fillValveData;
ValveData dumpValveData;
std::vector<ValveData*> valveDataVec({&fillValveData, &dumpValveData});
} // namespace GSDataCenter
