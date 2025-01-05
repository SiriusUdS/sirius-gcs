#ifndef LOGGINGWINDOW_H
#define LOGGINGWINDOW_H

#include <imgui.h>

namespace LoggingWindow {
void init();
void render();
void clear();
void addLog(const char* str, const char* strEnd);

extern bool autoScroll;
extern ImGuiTextBuffer buf;
extern ImGuiTextFilter filter;
extern ImVector<int> lineOffsets;
} // namespace LoggingWindow

#endif // LOGGINGWINDOW_H
