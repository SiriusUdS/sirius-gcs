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

SwitchData SwitchData1("Switch 1", false);
SwitchData SwitchData2("Switch 2", true);
SwitchData SwitchData3("Switch 3", false);
SwitchData SwitchData4("Switch 4", true);
SwitchData SwitchData5("Switch 5", false);
SwitchData SwitchData6("Switch 6", true);
SwitchData SwitchData7("Switch 7", false);
std::vector<SwitchData> SwitchDataVec({SwitchData1, SwitchData2, SwitchData3, SwitchData4, SwitchData5, SwitchData6, SwitchData7});
} // namespace GSDataCenter
