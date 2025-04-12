#include "ControlsWindow.h"

#include <imgui.h>

namespace ControlsWindow {
ValveState valveState1{ValveState::CLOSED};
ValveState valveState2{ValveState::CLOSED};
SerialCom serialTest{};
} // namespace ControlsWindow

namespace ControlsWindow {
void init() {
    serialTest.start();
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
    static char recvPacket[512] = {0};

    if (ImGui::CollapsingHeader("Serial")) {
        ImGui::Text("COM Opened: ");
        ImGui::SameLine();
        ImGui::Text(serialTest.comOpened() ? "Yes" : "No");

        if (ImGui::Button("Open COM")) {
            serialTest.start();
            if (serialTest.comOpened()) {
                GCS_LOG_INFO("ControlsWindow: COM opened.");
            } else {
                GCS_LOG_WARN("ControlsWindow: Couldn't open COM port.");
            }
        }

        if (ImGui::Button("Send test packet")) {
            bool success = serialTest.write(sendPacket, 12);
            if (success) {
                GCS_LOG_INFO("ControlsWindow: Sent following packet: {}", sendPacket);
            } else {
                GCS_LOG_WARN("ControlsWindow: Couldn't send packet.");
            }
        }

        if (ImGui::Button("Read incoming packet")) {
            size_t size = serialTest.getPacket(recvPacket);
            if (size > 0) {
                for (int i = 0; i < size; i++) {
                    if (recvPacket[i] == '\0') {
                        recvPacket[i] = '_';
                    }
                }
                recvPacket[size] = '\0';
                GCS_LOG_INFO("ControlsWindow: Received packet of size {}, contents are: {}", size, recvPacket);
            } else {
                GCS_LOG_WARN("ControlsWindow: Can't receive packet, no packets are available.");
            }
        }
    }

    serialTest.read();
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
    serialTest.shutdown();
}
} // namespace ControlsWindow
