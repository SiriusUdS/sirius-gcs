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

void ControlsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        for (size_t i = 0; i < GSDataCenter::VALVE_AMOUNT; i++) {
            renderValve(*GSDataCenter::ValveDataVec[i], i);
        }
    }

    if (ImGui::CollapsingHeader("Commands")) {
        ImGui::BeginDisabled(!CommandCenter::command.available());
        if (ImGui::Button("Send test command")) {
            CommandDispatch::test();
        }
        ImGui::EndDisabled();
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
