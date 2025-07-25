#ifndef GSDATACENTER_H
#define GSDATACENTER_H

#include <vector>

class SensorPlotData;
struct SwitchData;
struct ValveStateData;
struct ValveData;

namespace GSDataCenter {
constexpr size_t THERMISTOR_AMOUNT_PER_BOARD = 8;
constexpr size_t PRESSURE_SENSOR_AMOUNT_PER_BOARD = 2;
constexpr size_t LOAD_CELL_AMOUNT = 2;
constexpr size_t VALVE_AMOUNT = 4;

extern SensorPlotData Thermistor_Motor_PlotData[THERMISTOR_AMOUNT_PER_BOARD];
extern SensorPlotData PressureSensor_Motor_PlotData[PRESSURE_SENSOR_AMOUNT_PER_BOARD];
extern SensorPlotData Thermistor_FillingStation_PlotData[THERMISTOR_AMOUNT_PER_BOARD];
extern SensorPlotData PressureSensor_FillingStation_PlotData[PRESSURE_SENSOR_AMOUNT_PER_BOARD];
extern SensorPlotData LoadCell_FillingStation_PlotData[LOAD_CELL_AMOUNT];

extern SwitchData AllowDumpSwitchData;
extern SwitchData AllowFillSwitchData;
extern SwitchData ArmIgniterSwitchData;
extern SwitchData ArmServoSwitchData;
extern SwitchData EmergencyStopButtonData;
extern SwitchData FireIgniterButtonData;
extern SwitchData UnsafeKeySwitchData;
extern SwitchData ValveStartButtonData;

extern ValveData nosValveData;
extern ValveData ipaValveData;
extern ValveData fillValveData;
extern ValveData dumpValveData;
extern std::vector<ValveData*> valveDataVec;
} // namespace GSDataCenter

#endif // GSDATACENTER_H
