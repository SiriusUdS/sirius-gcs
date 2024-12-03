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
    ImGuiTextBuffer _buf;
    ImGuiTextFilter _filter;
    ImVector<int> _lineOffsets;
    bool _autoScroll;
};

#endif // LOGGINGWINDOW_H
