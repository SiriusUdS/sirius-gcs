#include "ControlsWindow.h"

#include "CommandControl.h"
#include "CommandTypes.h"
#include "Engine/EngineSensors.h"
#include "FillingStation/FillingStationSensors.h"
#include "GSDataCenter.h"
#include "Logging.h"
#include "SwitchData.h"
#include "ValveData.h"

#include <imgui.h>

namespace ControlsWindow {
struct ValveSlider {
    int openedValue_perc{};
    int lastOpenedValue_perc{};
    bool wasSliderEnabled = true;
};

ValveSlider fillValveSlider;
ValveSlider dumpValveSlider;
ValveSlider fillHeatPadSlider;
ValveSlider dumpHeatPadSlider;

static void renderSlider(const char* name, ValveSlider& slider, CommandType commandType, bool sliderEnabled = true);
} // namespace ControlsWindow

void ControlsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        renderSlider("Fill Valve", fillValveSlider, CommandType::FillValve, GSDataCenter::AllowFillSwitchData.isOn);
        renderSlider("Dump Valve", dumpValveSlider, CommandType::DumpValve, GSDataCenter::AllowDumpSwitchData.isOn);
        renderSlider("Fill Heat Pad", fillHeatPadSlider, CommandType::FillHeatPad);
        renderSlider("Dump Heat Pad", dumpHeatPadSlider, CommandType::DumpHeatPad);
    }
}

void ControlsWindow::renderSlider(const char* name, ValveSlider& slider, CommandType commandType, bool sliderEnabled) {
    ImGui::BeginDisabled(!sliderEnabled);
    ImGui::SliderInt(name, &slider.openedValue_perc, 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp);
    ImGui::EndDisabled();

    const bool sliderEnableChangedLastFrame = slider.wasSliderEnabled != sliderEnabled;
    const bool needToSynchronize = slider.openedValue_perc != slider.lastOpenedValue_perc;

    if (sliderEnabled && (sliderEnableChangedLastFrame || needToSynchronize)) {
        slider.wasSliderEnabled = sliderEnabled;
        slider.lastOpenedValue_perc = slider.openedValue_perc;
        CommandControl::sendCommand(commandType, slider.openedValue_perc);
    }
}
