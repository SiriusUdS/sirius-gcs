#include "ControlsWindow.h"

#include "SerialTask.h"

#include <imgui.h>

namespace ControlsWindow {
ValveState valveState1{ValveState::CLOSED};
ValveState valveState2{ValveState::CLOSED};
} // namespace ControlsWindow

namespace ControlsWindow {
void init() {
    // serialTest.start();
}

void render() {
    if (ImGui::CollapsingHeader("Valves")) {
        ImGui::TextUnformatted("Valve 1");
        ImGui::SameLine();
        renderValveState("valve1", valveState1);

        ImGui::TextUnformatted("Valve 2");
        ImGui::SameLine();
        renderValveState("valve2", valveState2);
    }

    static char sendPacket[13] = "bonjour hi! ";

    if (ImGui::CollapsingHeader("Serial")) {
        ImGui::Text("COM Opened: ");
        ImGui::SameLine();
        ImGui::Text(SerialTask::com.comOpened() ? "Yes" : "No");

        ImGui::Text("Packets read/s: %d", SerialTask::com.packetsReadPerSecond());

        if (ImGui::Button("Send test packet")) {
            bool success = SerialTask::com.write((uint8_t*) sendPacket, 12);
            if (success) {
                GCS_LOG_INFO("ControlsWindow: Sent following packet: {}", sendPacket);
            } else {
                GCS_LOG_WARN("ControlsWindow: Couldn't send packet.");
            }
        }
    }
}

void renderValveState(const char* id, ValveState state) {
    ImVec4 buttonColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    const char* buttonLabel = "Undefined";

    if (state == ValveState::OPEN) {
        buttonColor = ImVec4(0.0f, 0.6f, 0.0f, 1.0f);
        buttonLabel = "Open";
    } else if (state == ValveState::CLOSED) {
        buttonColor = ImVec4(0.6f, 0.0f, 0.0f, 1.0f);
        buttonLabel = "Closed";
    } else if (state == ValveState::OPENING) {
        buttonColor = ImVec4(0.0f, 0.6f, 0.0f, 1.0f);
        buttonLabel = "Opening";
    } else if (state == ValveState::CLOSING) {
        buttonColor = ImVec4(0.6f, 0.0f, 0.0f, 1.0f);
        buttonLabel = "Closing";
    }

    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
    ImGui::BeginDisabled();
    std::string buttonID = std::string(buttonLabel) + "##" + id;
    ImGui::Button(buttonID.c_str(), ImVec2(200.0f, 0.0f));
    ImGui::EndDisabled();
    ImGui::PopStyleColor(1);
}

void shutdown() {
    SerialTask::com.shutdown();
}
} // namespace ControlsWindow
