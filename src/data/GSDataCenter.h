#ifndef GSDATACENTER_H
#define GSDATACENTER_H

#include <vector>

class PlotData;
struct SwitchData;

namespace GSDataCenter {
extern PlotData Thermistor1PlotData;
extern PlotData Thermistor2PlotData;
extern PlotData Thermistor3PlotData;
extern PlotData Thermistor4PlotData;
extern PlotData Thermistor5PlotData;
extern PlotData Thermistor6PlotData;
extern PlotData Thermistor7PlotData;
extern PlotData Thermistor8PlotData;
extern PlotData PressureSensor1PlotData;
extern PlotData PressureSensor2PlotData;
extern PlotData LoadCell1PlotData;
extern PlotData LoadCell2PlotData;

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
