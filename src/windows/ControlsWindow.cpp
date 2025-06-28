#include "ControlsWindow.h"

#include "Command.h"
#include "CommandCenter.h"
#include "CommandDispatch.h"

#include <imgui.h>

namespace ControlsWindow {
int valveValues[3] = {0, 0, 0};
} // namespace ControlsWindow

void ControlsWindow::render() {
    if (ImGui::CollapsingHeader("Valves")) {
        // clang-format off
        if (ImGui::SliderInt("Valve 1", &valveValues[0], 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp) && CommandCenter::valve1Command.available()) {
            CommandDispatch::valve(CommandCenter::valve1Command);
        }
        if (ImGui::SliderInt("Valve 2", &valveValues[1], 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp) && CommandCenter::valve2Command.available()) {
            CommandDispatch::valve(CommandCenter::valve2Command);
        }
        if (ImGui::SliderInt("Valve 3", &valveValues[2], 0, 100, "%d%% Open", ImGuiSliderFlags_AlwaysClamp) && CommandCenter::valve3Command.available()) {
            CommandDispatch::valve(CommandCenter::valve3Command);
        }
        // clang-format on
    }

    if (ImGui::CollapsingHeader("Commands")) {
        ImGui::BeginDisabled(!CommandCenter::command.available());
        if (ImGui::Button("Send test command")) {
            CommandDispatch::test();
        }
        ImGui::EndDisabled();
    }
}
