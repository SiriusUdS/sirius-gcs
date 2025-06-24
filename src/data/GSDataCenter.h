#ifndef GSDATACENTER_H
#define GSDATACENTER_H

#include <vector>

class SensorPlotData;
struct SwitchData;

namespace GSDataCenter {
constexpr size_t THERMISTOR_AMOUNT = 8;
constexpr size_t PRESSURE_SENSOR_AMOUNT = 2;
constexpr size_t LOAD_CELL_AMOUNT = 2;

extern SensorPlotData ThermistorPlotData[THERMISTOR_AMOUNT];
extern SensorPlotData PressureSensorPlotData[PRESSURE_SENSOR_AMOUNT];
extern SensorPlotData LoadCellPlotData[LOAD_CELL_AMOUNT];

extern SwitchData AllowDumpSwitchData;
extern SwitchData AllowFillSwitchData;
extern SwitchData ArmIgniterSwitchData;
extern SwitchData ArmServoSwitchData;
extern SwitchData EmergencyStopButtonData;
extern SwitchData FireIgniterButtonData;
extern SwitchData UnsafeKeySwitchData;
extern SwitchData ValveStartButtonData;
extern std::vector<SwitchData*> SwitchDataVec;
} // namespace GSDataCenter

#endif // GSDATACENTER_H
