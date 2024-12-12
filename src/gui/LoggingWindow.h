#ifndef LOGGINGWINDOW_H
#define LOGGINGWINDOW_H

#include "Window.h"

#include <imgui.h>

class LoggingWindow : public Window {
public:
    explicit LoggingWindow();
    void renderContent();
    void clear();
    void addLog(const char* str, const char* strEnd);

private:
    ImGuiTextBuffer buf;
    ImGuiTextFilter filter;
    ImVector<int> lineOffsets;
    bool autoScroll;
};

#endif // LOGGINGWINDOW_H
