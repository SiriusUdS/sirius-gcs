#include "ControlsWindow.h"

#include "CommandControl.h"
#include "CommandTypes.h"
#include "Engine/EngineSensors.h"
#include "Engine/EngineState.h"
#include "FillingStation/FillingStationSensors.h"
#include "GSDataCenter.h"
#include "Logging.h"
#include "SwitchData.h"
#include "ValveData.h"

#include <imgui.h>

namespace ControlsWindow {
/**
 * @struct ValveSlider
 * @brief Holds the state of a valve slider.
 */
struct PercentageSlider {
    int openedValue_perc{};       ///< Current value of the slider in percentage.
    int lastOpenedValue_perc{};   ///< Last value sent to the command control.
    bool wasSliderEnabled = true; ///< Whether the slider was last enabled.
};

PercentageSlider nosValveSlider;
PercentageSlider ipaValveSlider;
PercentageSlider fillValveSlider;
PercentageSlider dumpValveSlider;
PercentageSlider nosHeatPadSlider;
PercentageSlider ipaHeatPadSlider;
PercentageSlider fillHeatPadSlider;
PercentageSlider dumpHeatPadSlider;

static void renderSlider(const char* name, PercentageSlider& slider, CommandType commandType, bool sliderEnabled = true);
} // namespace ControlsWindow

void ControlsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        const bool nosAndIpaValveSliderEnabled = GSDataCenter::motorBoardState == ENGINE_STATE_UNSAFE && GSDataCenter::ArmServoSwitchData.isOn
                                                 && !GSDataCenter::AllowDumpSwitchData.isOn && !GSDataCenter::AllowFillSwitchData.isOn
                                                 && !GSDataCenter::ArmIgniterSwitchData.isOn;
        const bool fillValveSliderEnabled = GSDataCenter::AllowFillSwitchData.isOn;
        const bool dumpValveSliderEnabled = GSDataCenter::AllowDumpSwitchData.isOn;

        renderSlider("NOS Valve", nosValveSlider, CommandType::NosValve, nosAndIpaValveSliderEnabled);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !nosAndIpaValveSliderEnabled) {
            ImGui::SetTooltip("To control the NOS valve -> [UNSAFE, ARM VALVE] need to be ON, [DUMP, FILL, ARM IGNITER] need to be OFF.");
        }

        renderSlider("IPA Valve", ipaValveSlider, CommandType::IpaValve, nosAndIpaValveSliderEnabled);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !nosAndIpaValveSliderEnabled) {
            ImGui::SetTooltip("To control the IPA valve -> [UNSAFE, ARM VALVE] need to be ON, [DUMP, FILL, ARM IGNITER] need to be OFF.");
        }

        renderSlider("Fill Valve", fillValveSlider, CommandType::FillValve, GSDataCenter::AllowFillSwitchData.isOn);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !fillValveSliderEnabled) {
            ImGui::SetTooltip("To control the FILL valve -> [UNSAFE, FILL] need to be ON.");
        }
        renderSlider("Dump Valve", dumpValveSlider, CommandType::DumpValve, GSDataCenter::AllowDumpSwitchData.isOn);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !dumpValveSliderEnabled) {
            ImGui::SetTooltip("To control the DUMP valve -> [UNSAFE, DUMP] need to be ON.");
        }
    }

    if (ImGui::CollapsingHeader("Heat pads")) {
        renderSlider("Nos Heat Pad", nosHeatPadSlider, CommandType::NosHeatPad);
        renderSlider("Ipa Heat Pad", ipaHeatPadSlider, CommandType::IpaHeatPad);
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

void ControlsWindow::renderSlider(const char* name, PercentageSlider& slider, CommandType commandType, bool sliderEnabled) {
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
