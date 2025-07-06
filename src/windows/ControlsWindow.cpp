#include "ControlsWindow.h"

#include "Command.h"
#include "CommandCenter.h"
#include "CommandDispatch.h"

#include <imgui.h>

namespace ControlsWindow {
int valveValues[3] = {0, 0, 0};
int valveValuesLastSent[3] = {0, 0, 0};
} // namespace ControlsWindow

void ControlsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        bool sliderChanged, commandAvailable, needToSynchronize;

        commandAvailable = CommandCenter::valve1Command.available();
        sliderChanged = ImGui::SliderInt("Valve 1", &valveValues[0], 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp);
        needToSynchronize = valveValues[0] != valveValuesLastSent[0];
        if (commandAvailable && (sliderChanged || needToSynchronize)) {
            valveValuesLastSent[0] = valveValues[0];
            CommandDispatch::valve(CommandCenter::valve1Command, valveValues[0]);
        }

        commandAvailable = CommandCenter::valve2Command.available();
        sliderChanged = ImGui::SliderInt("Valve 2", &valveValues[1], 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp);
        needToSynchronize = valveValues[1] != valveValuesLastSent[1];
        if (commandAvailable && (sliderChanged || needToSynchronize)) {
            valveValuesLastSent[1] = valveValues[1];
            CommandDispatch::valve(CommandCenter::valve2Command, valveValues[1]);
        }

        commandAvailable = CommandCenter::valve3Command.available();
        sliderChanged = ImGui::SliderInt("Valve 3", &valveValues[2], 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp);
        needToSynchronize = valveValues[2] != valveValuesLastSent[2];
        if (commandAvailable && (sliderChanged || needToSynchronize)) {
            valveValuesLastSent[2] = valveValues[2];
            CommandDispatch::valve(CommandCenter::valve3Command, valveValues[2]);
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
