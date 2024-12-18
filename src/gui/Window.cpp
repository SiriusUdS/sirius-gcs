#include "Window.h"

#include <imgui.h>

Window::Window(const char* name) : name(name), visible(true) {
}

void Window::render() {
    if (!visible)
        return;

    ImGui::Begin(name, &visible);
    renderContent();
    ImGui::End();
}

void Window::loadState(const mINI::INIStructure& ini) {
}

void Window::saveState(mINI::INIStructure& ini) const {
}
