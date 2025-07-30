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
ValveSlider nosValveSlider;
ValveSlider ipaValveSlider;
ValveSlider fillHeatPadSlider;
ValveSlider dumpHeatPadSlider;

static void renderSlider(const char* name, ValveSlider& slider, CommandType commandType, bool sliderEnabled = true);
} // namespace ControlsWindow

void ControlsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        renderSlider("Fill Valve", fillValveSlider, CommandType::FillValve, GSDataCenter::AllowFillSwitchData.isOn);
        renderSlider("Dump Valve", dumpValveSlider, CommandType::DumpValve, GSDataCenter::AllowDumpSwitchData.isOn);
    }

    if (ImGui::CollapsingHeader("Heat pads")) {
        renderSlider("Nos Heat Pad", nosValveSlider, CommandType::NosHeatPad);
        renderSlider("Ipa Heat Pad", ipaValveSlider, CommandType::IpaHeatPad);
        renderSlider("Fill Heat Pad", fillHeatPadSlider, CommandType::FillHeatPad);
        renderSlider("Dump Heat Pad", dumpHeatPadSlider, CommandType::DumpHeatPad);
    }

    if (ImGui::CollapsingHeader("Reset & Abort")) {
        if (ImGui::Button("Reset")) {
            CommandControl::sendCommand(CommandType::Reset, 0);
        }
        ImGui::SameLine();
        if (ImGui::Button("Abort")) {
            CommandControl::sendCommand(CommandType::Abort, 0);
        }
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
