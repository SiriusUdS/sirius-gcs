#include "ControlsWindow.h"

#include "Command.h"
#include "CommandCenter.h"
#include "CommandDispatch.h"
#include "GSDataCenter.h"
#include "ValveData.h"

#include <imgui.h>

namespace ControlsWindow {
int valveValues[GSDataCenter::VALVE_AMOUNT] = {0, 0, 0, 0};
int valveValuesLastSent[GSDataCenter::VALVE_AMOUNT] = {0, 0, 0, 0};

void renderValve(ValveData& valveData, size_t idx);
} // namespace ControlsWindow

void tempShowValve(ValveData& data) {
    ImGui::Text(data.name);
    ImGui::Text("Is Idle: %s", data.isIdle ? "Yes" : "No");
    ImGui::Text("Is Opened Switch High: %s", data.openedSwitchHigh ? "Yes" : "No");
    ImGui::Text("Is Closed Switch High: %s", data.closedSwitchHigh ? "Yes" : "No");
    ImGui::Separator();
}

void ControlsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        for (size_t i = 0; i < GSDataCenter::VALVE_AMOUNT; i++) {
            renderValve(*GSDataCenter::ValveDataVec[i], i);
        }
    }

    if (ImGui::CollapsingHeader("Heat pads")) {
        if (ImGui::Button("NOS valve heat pad")) {
            CommandDispatch::heatPad(CommandDispatch::HeatPadCommandType::NOS, 50);
        }
        if (ImGui::Button("IPA valve heat pad")) {
            CommandDispatch::heatPad(CommandDispatch::HeatPadCommandType::IPA, 50);
        }
        if (ImGui::Button("Fill valve heat pad")) {
            CommandDispatch::heatPad(CommandDispatch::HeatPadCommandType::FILL, 50);
        }
        if (ImGui::Button("Dump valve heat pad")) {
            CommandDispatch::heatPad(CommandDispatch::HeatPadCommandType::DUMP, 50);
        }
    }

    if (ImGui::CollapsingHeader("Commands")) {
        ImGui::BeginDisabled(!CommandCenter::command.available());
        if (ImGui::Button("Send test command")) {
            CommandDispatch::test();
        }
        ImGui::EndDisabled();
    }

    if (ImGui::CollapsingHeader("Temp valves")) {
        tempShowValve(GSDataCenter::NosValveData);
        tempShowValve(GSDataCenter::IpaValveData);
        tempShowValve(GSDataCenter::FillValveData);
        tempShowValve(GSDataCenter::DumpValveData);
    }
}

void ControlsWindow::renderValve(ValveData& valveData, size_t idx) {
    const bool commandAvailable = CommandCenter::valve1Command.available();
    const bool sliderChanged = ImGui::SliderInt(valveData.name, &valveValues[idx], 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp);
    const bool needToSynchronize = valveValues[idx] != valveValuesLastSent[idx];

    if (commandAvailable && (sliderChanged || needToSynchronize)) {
        valveValuesLastSent[idx] = valveValues[idx];
        CommandDispatch::valve(CommandCenter::valve1Command, valveValues[idx]);
    }
}
