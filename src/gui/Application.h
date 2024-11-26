#ifndef APPLICATION_H
#define APPLICATION_H

#include "LoggingWindow.h"
#include "MapWindow.h"
#include "PlotWindow.h"

namespace Application {
void init();
void render();
void shutdown();

extern std::unique_ptr<LoggingWindow> loggingWindow;
extern std::unique_ptr<MapWindow> mapWindow;
extern std::unique_ptr<PlotWindow> plotWindow;
} // namespace Application

#endif // APPLICATION_H
