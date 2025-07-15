#include "GSDataCenter.h"

#include "Engine/EngineSensors.h"
#include "FillingStation/FillingStationSensors.h"
#include "PlotColors.h"
#include "SensorPlotData.h"
#include "SwitchData.h"
#include "ValveData.h"
#include "ValveStateData.h"

namespace GSDataCenter {
SensorPlotData Thermistor_Motor_PlotData[THERMISTOR_AMOUNT_PER_BOARD] = {{"Thermistor 1", PlotColors::BLUE},   {"Thermistor 2", PlotColors::RED},
                                                                         {"Thermistor 3", PlotColors::GREEN},  {"Thermistor 4", PlotColors::YELLOW},
                                                                         {"Thermistor 5", PlotColors::ORANGE}, {"Thermistor 6", PlotColors::CYAN},
                                                                         {"Thermistor 7", PlotColors::PURPLE}, {"Thermistor 8", PlotColors::GRAY}};
SensorPlotData PressureSensor_Motor_PlotData[PRESSURE_SENSOR_AMOUNT_PER_BOARD] = {{"Pressure Sensor 1", PlotColors::BLUE},
                                                                                  {"Pressure Sensor 2", PlotColors::RED}};
SensorPlotData Thermistor_FillingStation_PlotData[THERMISTOR_AMOUNT_PER_BOARD] = {
  {"Thermistor 1", PlotColors::BLUE},   {"Thermistor 2", PlotColors::RED},    {"Thermistor 3", PlotColors::GREEN},
  {"Thermistor 4", PlotColors::YELLOW}, {"Thermistor 5", PlotColors::ORANGE}, {"Thermistor 6", PlotColors::CYAN},
  {"Thermistor 7", PlotColors::PURPLE}, {"Thermistor 8", PlotColors::GRAY}};
SensorPlotData PressureSensor_FillingStation_PlotData[PRESSURE_SENSOR_AMOUNT_PER_BOARD] = {{"Pressure Sensor 1", PlotColors::BLUE},
                                                                                           {"Pressure Sensor 2", PlotColors::RED}};
SensorPlotData LoadCell_FillingStation_PlotData[LOAD_CELL_AMOUNT] = {{"Motor Load Cell", PlotColors::BLUE}, {"Tank Load Cell", PlotColors::RED}};

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

ValveData ValveEngineData[GSDataCenter::VALVE_AMOUNT] = {
    {.valveStateData = ValveStateData{}, .id = ENGINE_IPA_VALVE_INDEX, .name = "IPA Valve", .openedValue_perc = 0, .lastOpenedValue_perc = 0},
    {.valveStateData = ValveStateData{}, .id = ENGINE_NOS_VALVE_INDEX, .name = "NOS Valve", .openedValue_perc = 0, .lastOpenedValue_perc = 0}
};

ValveData ValveFillStationData[GSDataCenter::VALVE_AMOUNT] = {
    {.valveStateData = ValveStateData{}, .id = FILLING_STATION_NOS_VALVE_INDEX, .name = "Fill Valve", .openedValue_perc = 0, .lastOpenedValue_perc = 0},
    {.valveStateData = ValveStateData{}, .id = FILLING_STATION_NOS_DUMP_VALVE_INDEX, .name = "Dump Valve", .openedValue_perc = 0, .lastOpenedValue_perc = 0}
};

} // namespace GSDataCenter
