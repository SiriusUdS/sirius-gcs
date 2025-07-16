#include "ControlsWindow.h"

#include "Command.h"
#include "CommandCenter.h"
#include "CommandDispatch.h"
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

static void renderValve(ValveData& data, ValveSlider& slider, int valveId, bool sliderEnabled);
} // namespace ControlsWindow

void ControlsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        renderValve(GSDataCenter::fillValveData, fillValveSlider, FILLING_STATION_COMMAND_CODE_OPEN_FILL_VALVE_PCT,
                    GSDataCenter::AllowFillSwitchData.isOn);
        renderValve(GSDataCenter::dumpValveData, dumpValveSlider, FILLING_STATION_COMMAND_CODE_OPEN_DUMP_VALVE_PCT,
                    GSDataCenter::AllowDumpSwitchData.isOn);
    }

    if (ImGui::CollapsingHeader("Heat pads")) {
        if (ImGui::Button("Fill valve heat pad")) {
            CommandDispatch::heatPad(CommandDispatch::HeatPadCommandType::FILL, 50);
        }
        if (ImGui::Button("Dump valve heat pad")) {
            CommandDispatch::heatPad(CommandDispatch::HeatPadCommandType::DUMP, 50);
        }
    }
}

void ControlsWindow::renderValve(ValveData& data, ValveSlider& slider, int valveId, bool sliderEnabled) {
    ImGui::BeginDisabled(!sliderEnabled);
    ImGui::SliderInt(data.name, &slider.openedValue_perc, 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp);
    ImGui::EndDisabled();

    const bool sliderEnableChangedLastFrame = slider.wasSliderEnabled != sliderEnabled;
    const bool needToSynchronize = slider.openedValue_perc != slider.lastOpenedValue_perc;

    if (sliderEnabled && (sliderEnableChangedLastFrame || needToSynchronize)) {
        if (CommandCenter::valveCommand.available()) {
            slider.wasSliderEnabled = sliderEnabled;
            slider.lastOpenedValue_perc = slider.openedValue_perc;
            CommandDispatch::valve(valveId, slider.openedValue_perc);
        }
    }
}
