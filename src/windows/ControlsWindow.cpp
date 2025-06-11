#include "ControlsWindow.h"

#include "CommandCenter.h"
#include "CommandDispatch.h"
#include "PacketRateMonitor.h"
#include "SerialFailureMonitor.h"
#include "SerialTask.h"

#include <imgui.h>

namespace ControlsWindow {
void render() {
    if (ImGui::CollapsingHeader("Serial")) {
        ImGui::Text("COM Opened: ");
        ImGui::SameLine();
        ImGui::Text(SerialTask::com.comOpened() ? "Yes" : "No");
        ImGui::Text("COM Working: ");
        ImGui::SameLine();
        ImGui::Text(SerialTask::com.comOpened() && SerialTask::serialFailureMonitor.isComWorking() ? "Yes" : "No");

        ImGui::Text("Packets read/s: %d", SerialTask::packetRateMonitor.getRatePerSecond());

        ImGui::BeginDisabled(!CommandCenter::available());
        if (ImGui::Button("Send test command")) {
            CommandDispatch::test();
        }
        ImGui::EndDisabled();
    }
}
} // namespace ControlsWindow
