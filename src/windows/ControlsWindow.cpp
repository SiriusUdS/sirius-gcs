#include "ControlsWindow.h"

#include "CommandCenter.h"
#include "CommandDispatch.h"
#include "PacketRateMonitor.h"
#include "SerialStateMonitor.h"
#include "SerialTask.h"

#include <imgui.h>

namespace ControlsWindow {
void render() {
    if (ImGui::CollapsingHeader("Serial")) {
        const char* comStateText = "Unknown";
        if (SerialTask::com.comOpened()) {
            comStateText = "Disconnected";
        } else {
            switch (SerialTask::serialFailureMonitor.getState()) {
            case SerialStateMonitor::State::STARTING:
                comStateText = "Starting";
                break;
            case SerialStateMonitor::State::RESETTING:
                comStateText = "Resetting";
                break;
            case SerialStateMonitor::State::WORKING:
                comStateText = "Working";
                break;
            case SerialStateMonitor::State::NOT_WORKING:
                comStateText = "Not working";
                break;
            }
        }

        ImGui::Text("COM State: ");
        ImGui::SameLine();
        ImGui::Text(comStateText);

        ImGui::Text("Packets read/s: %.1f", SerialTask::packetRateMonitor.getRatePerSecond());

        ImGui::BeginDisabled(!CommandCenter::available());
        if (ImGui::Button("Send test command")) {
            CommandDispatch::test();
        }
        ImGui::EndDisabled();
    }
}
} // namespace ControlsWindow
