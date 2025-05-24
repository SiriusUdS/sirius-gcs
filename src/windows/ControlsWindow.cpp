#include "ControlsWindow.h"

#include "CommandDispatch.h"
#include "SerialTask.h"

#include <imgui.h>

namespace ControlsWindow {
ValveState valveState1{ValveState::CLOSED};
ValveState valveState2{ValveState::CLOSED};
} // namespace ControlsWindow

namespace ControlsWindow {
void render() {
    if (ImGui::CollapsingHeader("Serial")) {
        ImGui::Text("COM Opened: ");
        ImGui::SameLine();
        ImGui::Text(SerialTask::com.comOpened() ? "Yes" : "No");

        ImGui::Text("Packets read/s: %d", SerialTask::com.packetsReadPerSecond());
        // ImGui::Text("Loop time SerialTask: %2.6f", SerialTask::secondsSinceLastUpdate());

        if (ImGui::Button("Send test command")) {
            CommandDispatch::test();
        }
    }
}
} // namespace ControlsWindow
