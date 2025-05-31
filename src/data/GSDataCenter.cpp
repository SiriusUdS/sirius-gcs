#include "GSDataCenter.h"

#include "PlotColors.h"
#include "PlotData.h"
#include "SwitchData.h"

namespace GSDataCenter {
PlotData Thermistor1PlotData("Thermistor 1", PlotColors::BLUE);
PlotData Thermistor2PlotData("Thermistor 2", PlotColors::RED);
PlotData Thermistor3PlotData("Thermistor 3", PlotColors::GREEN);
PlotData Thermistor4PlotData("Thermistor 4", PlotColors::YELLOW);
PlotData Thermistor5PlotData("Thermistor 5", PlotColors::ORANGE);
PlotData Thermistor6PlotData("Thermistor 6", PlotColors::CYAN);
PlotData Thermistor7PlotData("Thermistor 7", PlotColors::PURPLE);
PlotData Thermistor8PlotData("Thermistor 8", PlotColors::GRAY);
PlotData PressureSensor1PlotData("Pressure Sensor 1", PlotColors::BLUE);
PlotData PressureSensor2PlotData("Pressure Sensor 2", PlotColors::RED);
PlotData LoadCell1PlotData("Load Cell 1", PlotColors::BLUE);
PlotData LoadCell2PlotData("Load Cell 2", PlotColors::RED);

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
