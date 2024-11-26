#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H

#include "ControlsWindow.h"
#include "LoggingWindow.h"

#include <memory>

namespace ImGuiManager {
void init();
void render();
void shutdown();

extern std::unique_ptr<ControlsWindow> controlsWindow;
extern std::unique_ptr<LoggingWindow> loggingWindow;
} // namespace ImGuiManager

#endif // IMGUIMANAGER_H
