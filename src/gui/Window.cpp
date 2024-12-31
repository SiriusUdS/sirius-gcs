#include "Window.h"

#include <imgui.h>

Window::Window() {
}

void Window::render() {
    ImGui::Begin(getName().c_str(), &isVisible());
    renderContent();
    ImGui::End();
}

void Window::loadState(const mINI::INIStructure& ini) {
}

void Window::saveState(mINI::INIStructure& ini) const {
}

std::string Window::getName() {
    return HelloImGui::GetRunnerParams()->dockingParams.dockableWindows[dockableWindowIndex].label;
}

bool& Window::isVisible() {
    return HelloImGui::GetRunnerParams()->dockingParams.dockableWindows[dockableWindowIndex].isVisible;
}
