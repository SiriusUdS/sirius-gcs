#ifndef LOGGINGWINDOW_H
#define LOGGINGWINDOW_H

#include <imgui.h>
#include <ini.h>

namespace LoggingWindow {
void render();
void loadState(const mINI::INIStructure& ini);
void saveState(mINI::INIStructure& ini);
void clear();
void addLog(const char* str, const char* strEnd);

extern bool autoScroll;
extern ImGuiTextBuffer buf;
extern ImGuiTextFilter filter;
extern ImVector<int> lineOffsets;
} // namespace LoggingWindow

#endif // LOGGINGWINDOW_H
