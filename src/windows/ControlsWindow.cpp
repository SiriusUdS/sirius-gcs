#include "ControlsWindow.h"

#include "CommandCenter.h"
#include "CommandDispatch.h"
#include "FontConfig.h"
#include "PacketCircularBuffer.h"
#include "PacketRateMonitor.h"
#include "SerialStateMonitor.h"
#include "SerialTask.h"

#include <algorithm>
#include <imgui.h>

namespace ControlsWindow {
void recvBufferContentModal();

std::vector<char> recvBufferContentDisplay(PACKET_CIRCULAR_BUFFER_SIZE);
} // namespace ControlsWindow

void ControlsWindow::render() {
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

        ImGui::BeginDisabled(!CommandCenter::available());
        if (ImGui::Button("Send test command")) {
            CommandDispatch::test();
        }
        ImGui::EndDisabled();

        if (ImGui::Button("View RECV buffer content")) {
            ImGui::OpenPopup("RECV Buffer Content");
        }

        recvBufferContentModal();
    }
}

void ControlsWindow::recvBufferContentModal() {
    const char* buf = (char*) SerialTask::com.getBuffer();

    for (size_t i = 0; i < PACKET_CIRCULAR_BUFFER_SIZE; i++) {
        if (buf[i] == '\0') {
            recvBufferContentDisplay[i] = '~';
        } else {
            recvBufferContentDisplay[i] = buf[i];
        }
    }

    const ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    const ImVec2 minModalSize = {400, 200};
    const ImVec2 maxModalSize = {2000, 1200};
    const ImVec2 modalSize = {std::clamp(screenSize.x * 0.8f, minModalSize.x, maxModalSize.x),
                              std::clamp(screenSize.y * 0.6f, minModalSize.y, maxModalSize.y)};
    const ImVec2 modalPos = {(screenSize.x - modalSize.x) * 0.5f, (screenSize.y - modalSize.y) * 0.5f};

    ImGui::SetNextWindowSize(modalSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(modalPos, ImGuiCond_Always);

    const ImGuiWindowFlags modalFlags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove;

    if (ImGui::BeginPopupModal("RECV Buffer Content", nullptr, modalFlags)) {
        const ImVec2 boxSize = {modalSize.x - 25.0f, modalSize.y - 125.0f};
        const char* displayBuf = recvBufferContentDisplay.data();

        ImGui::BeginChild("RecvBufferBox", boxSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushTextWrapPos(0.0f);
        ImGui::PushFont(FontConfig::codeFont);
        ImGui::TextUnformatted(displayBuf, displayBuf + PACKET_CIRCULAR_BUFFER_SIZE);
        ImGui::PopFont();
        ImGui::PopTextWrapPos();
        ImGui::EndChild();

        if (ImGui::Button("Close", {boxSize.x, 0})) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
