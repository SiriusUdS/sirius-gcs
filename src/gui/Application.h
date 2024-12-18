#ifndef APPLICATION_H
#define APPLICATION_H

#include "LoggingWindow.h"
#include "MapWindow.h"
#include "PlotWindow.h"

#include <ini.h>

namespace Application {
void loadFonts();
void init();
void menuItems();
void render();
void shutdown();

extern mINI::INIFile iniFile;
extern mINI::INIStructure iniStructure;
extern std::shared_ptr<LoggingWindow> loggingWindow;
extern std::shared_ptr<MapWindow> mapWindow;
extern std::shared_ptr<PlotWindow> plotWindow;
extern std::vector<std::shared_ptr<Window>> windows;
} // namespace Application

#endif // APPLICATION_H
