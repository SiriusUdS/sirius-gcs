#include "ControlsWindow.h"

#include <imgui.h>

namespace ControlsWindow {
ValveState valveState1{ValveState::CLOSED};
std::chrono::time_point<std::chrono::system_clock> valveTimer1{};
ValveState valveState2{ValveState::CLOSED};
std::chrono::time_point<std::chrono::system_clock> valveTimer2{};
SerialTest serialTest{};
} // namespace ControlsWindow

bool ValveButton(const char* id, ValveState state) {
    bool buttonPressed = false;
    bool buttonDisabled = false;
    ImVec4 buttonColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 buttonHoveredColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 buttonPressedColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    const char* buttonLabel = "Undefined";

    if (state == ValveState::OPEN) {
        buttonColor = ImVec4(0.0f, 0.6f, 0.0f, 1.0f);
        buttonHoveredColor = ImVec4(0.0f, 0.5f, 0.0f, 1.0f);
        buttonPressedColor = ImVec4(0.0f, 0.4f, 0.0f, 1.0f);
        buttonLabel = "Open";
    } else if (state == ValveState::CLOSED) {
        buttonColor = ImVec4(0.6f, 0.0f, 0.0f, 1.0f);
        buttonHoveredColor = ImVec4(0.5f, 0.0f, 0.0f, 1.0f);
        buttonPressedColor = ImVec4(0.4f, 0.0f, 0.0f, 1.0f);
        buttonLabel = "Closed";
    } else if (state == ValveState::OPENING) {
        buttonColor = ImVec4(0.0f, 0.6f, 0.0f, 1.0f);
        buttonLabel = "Opening";
        buttonDisabled = true;
    } else if (state == ValveState::CLOSING) {
        buttonColor = ImVec4(0.6f, 0.0f, 0.0f, 1.0f);
        buttonLabel = "Closing";
        buttonDisabled = true;
    }

    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoveredColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonPressedColor);
    ImGui::BeginDisabled(buttonDisabled);
    std::string buttonID = std::string(buttonLabel) + "##" + id;
    buttonPressed = ImGui::Button(buttonID.c_str(), ImVec2(200.0f, 0.0f));
    ImGui::EndDisabled();
    ImGui::PopStyleColor(3);
    return buttonPressed;
}

void updateValveState(ValveState& state, std::chrono::time_point<std::chrono::system_clock>& timer, bool buttonPressed) {
    auto now = std::chrono::system_clock::now();

    if (state == ValveState::OPEN) {
        if (buttonPressed) {
            state = ValveState::CLOSING;
            timer = now + std::chrono::seconds(1);
        }
    } else if (state == ValveState::CLOSED) {
        if (buttonPressed) {
            state = ValveState::OPENING;
            timer = now + std::chrono::seconds(1);
        }
    } else if (state == ValveState::OPENING) {
        if (now >= timer) {
            state = ValveState::OPEN;
        }
    } else if (state == ValveState::CLOSING) {
        if (now >= timer) {
            state = ValveState::CLOSED;
        }
    }
}

namespace ControlsWindow {
void render() {
    if (ImGui::CollapsingHeader("Valves")) {
        ImGui::TextUnformatted("Valve 1");
        ImGui::SameLine();
        bool valveButton1Pressed = ValveButton("valve1", valveState1);
        updateValveState(valveState1, valveTimer1, valveButton1Pressed);

        ImGui::TextUnformatted("Valve 2");
        ImGui::SameLine();
        bool valveButton2Pressed = ValveButton("valve2", valveState2);
        updateValveState(valveState2, valveTimer2, valveButton2Pressed);
    }

    if (ImGui::CollapsingHeader("Serial")) {
        if (ImGui::Button("Initialize Serial Port")) {
            serialTest.init();
        }

        if (ImGui::Button("Perform Serial Test")) {
            serialTest.performTest();
        }

        if (ImGui::Button("Close Serial Port")) {
            serialTest.shutdown();
        }
    }
}
} // namespace ControlsWindow
