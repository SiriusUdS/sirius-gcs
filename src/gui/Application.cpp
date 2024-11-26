#include "Application.h"

#include "Logging.h"

#include <imgui.h>
#include <implot.h>

namespace Application {
std::unique_ptr<MapWindow> mapWindow;
std::unique_ptr<PlotWindow> plotWindow;
std::unique_ptr<LoggingWindow> loggingWindow;
} // namespace Application

void Application::init() {
    ImPlot::CreateContext();

    mapWindow = std::make_unique<MapWindow>();
    plotWindow = std::make_unique<PlotWindow>();
    loggingWindow = std::make_unique<LoggingWindow>();

    Logging::linkLoggingWindow(loggingWindow.get());
}

void Application::render() {
    mapWindow->draw();
    plotWindow->draw();
    loggingWindow->draw();
}

void Application::shutdown() {
    mapWindow = nullptr;
    plotWindow = nullptr;
    loggingWindow = nullptr;

    Logging::removeLoggingWindow();

    ImPlot::DestroyContext();
}