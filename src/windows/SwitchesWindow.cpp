#include "SwitchesWindow.h"

#include "GSDataCenter.h"
#include "SwitchData.h"

#include <imgui.h>
#include <string>

void SwitchesWindow::render() {
    for (const SwitchData& switchData : GSDataCenter::SwitchDataVec) {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", switchData.name);
        ImGui::SameLine();
        showSwitchState(switchData);
    }
}

void SwitchesWindow::showSwitchState(const SwitchData& data) {
    std::string label = data.isOn ? "ON###" : "OFF###";
    label += data.name;
    ImVec4 color = data.isOn ? ImVec4(0.0f, 0.45f, 0.0f, 1.0f)  // Green
                             : ImVec4(0.45f, 0.0f, 0.0f, 1.0f); // Red

    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

    ImGui::Button(label.c_str(), ImVec2(180, 0));

    ImGui::PopStyleColor(3);
}
