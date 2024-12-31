#include "LedWindow.h"

#include "Constants.h"
#include "Led.h"

LedWindow::LedWindow() : Window() {
}

void LedWindow::renderContent() {
    const ImVec2 small(50, 50);
    const ImVec2 big(100, 100);

    if (ImGui::Button("Switch")) {
        open = !open;
    }

    Led(open, small, LED_RED);
    ImGui::SameLine();
    Led(!open, small, LED_ORANGE);
    ImGui::SameLine();
    Led(open, small, LED_YELLOW);

    Led(!open, big, LED_GREEN);
    ImGui::SameLine();
    Led(open, big, LED_BLUE);
    ImGui::SameLine();
    Led(!open, big, LED_PURPLE);
}
