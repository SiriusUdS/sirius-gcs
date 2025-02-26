#include "ControlsWindow.h"

#include <imgui.h>

namespace ControlsWindow {
void render() {
    ImGui::TextUnformatted("Valve 1");
    ImGui::SameLine();
    if (ImGui::Button("Opened")) {
    }

    ImGui::TextUnformatted("Valve 2");
    ImGui::SameLine();
    if (ImGui::Button("Closed")) {
    }
}
} // namespace ControlsWindow
