#ifndef APPLICATION_H
#define APPLICATION_H

#include "LedWindow.h"
#include "LoggingWindow.h"
#include "MapWindow.h"
#include "PlotWindow.h"

#include <hello_imgui/hello_imgui.h>
#include <ini.h>

namespace Application {
void loadFonts();
void init();
void shutdown();

std::vector<HelloImGui::DockingSplit> createBaseDockingSplits();
std::vector<HelloImGui::DockableWindow> createDockableWindows();

extern mINI::INIFile iniFile;
extern mINI::INIStructure iniStructure;
extern std::shared_ptr<LedWindow> ledWindow;
extern std::shared_ptr<LoggingWindow> loggingWindow;
extern std::shared_ptr<MapWindow> mapWindow;
extern std::shared_ptr<PlotWindow> plotWindow;
extern std::vector<std::shared_ptr<Window>> windows;
} // namespace Application

#endif // APPLICATION_H
