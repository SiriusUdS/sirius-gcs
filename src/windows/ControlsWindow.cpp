#include "ControlsWindow.h"

#include "Command.h"
#include "CommandCenter.h"
#include "CommandDispatch.h"
#include "FontConfig.h"
#include "PacketCircularBuffer.h"
#include "PacketRateMonitor.h"
#include "SerialConfig.h"
#include "SerialStateMonitor.h"
#include "SerialTask.h"

#include <algorithm>
#include <imgui.h>

namespace ControlsWindow {
void recvBufferContentModal();

std::vector<char> recvBufferContentDisplay(SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE);
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

    if (ImGui::CollapsingHeader("Serial")) {
        const char* comStateText = "Unknown";
        if (!SerialTask::com.comOpened()) {
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

        if (ImGui::Button("View RECV buffer content")) {
            ImGui::OpenPopup("RECV Buffer Content");
        }

        recvBufferContentModal();
    }
}

void ControlsWindow::recvBufferContentModal() {
    const char* buf = (char*) SerialTask::com.getBuffer();

    for (size_t i = 0; i < SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE; i++) {
        if (buf[i] == '\0') {
            recvBufferContentDisplay[i] = '~';
        } else {
            recvBufferContentDisplay[i] = buf[i];
        }
    }

    const ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    const ImVec2 minBoxSize = {400, 200};
    const ImVec2 maxBoxSize = {2000, 1200};
    const ImVec2 boxSize = {std::clamp(screenSize.x * 0.8f, minBoxSize.x, maxBoxSize.x), std::clamp(screenSize.y * 0.6f, minBoxSize.y, maxBoxSize.y)};
    const ImVec2 boxPos = {(screenSize.x - boxSize.x) * 0.5f - 10.0f, (screenSize.y - boxSize.y) * 0.5f - 60.0f};

    ImGui::SetNextWindowPos(boxPos, ImGuiCond_Always);

    const ImGuiWindowFlags modalFlags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove;

    if (ImGui::BeginPopupModal("RECV Buffer Content", nullptr, modalFlags)) {
        const char* displayBuf = recvBufferContentDisplay.data();

        ImGui::BeginChild("RecvBufferBox", boxSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushTextWrapPos(0.0f);
        ImGui::PushFont(FontConfig::codeFont);
        ImGui::TextUnformatted(displayBuf, displayBuf + SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE);
        ImGui::PopFont();
        ImGui::PopTextWrapPos();
        ImGui::EndChild();

        if (ImGui::Button("Close", {boxSize.x, 0})) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
