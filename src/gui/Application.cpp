#include "Application.h"

#include "Altimeter/AltimeterData.h"
#include "Constants.h"
#include "Logging.h"

#include <WinSock2.h>
#include <imgui.h>
#include <implot.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

namespace Application {
mINI::INIFile iniFile(Constants::GCS_INI_FILENAME);
mINI::INIStructure iniStructure;
std::shared_ptr<MapWindow> mapWindow;
std::shared_ptr<PlotWindow> plotWindow;
std::shared_ptr<LoggingWindow> loggingWindow;
std::vector<std::shared_ptr<Window>> windows;
} // namespace Application

void Application::loadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(Constants::IMGUI_MAIN_FONT_FILENAME, Constants::IMGUI_MAIN_FONT_SIZE);
}

void Application::init() {
    Logging::initSpdLog();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        GCS_LOG_ERROR("WSAStartup failed");
        return;
    }

    ImPlot::CreateContext();

    mapWindow = std::make_shared<MapWindow>();
    plotWindow = std::make_shared<PlotWindow>();
    loggingWindow = std::make_shared<LoggingWindow>();
    windows = {mapWindow, plotWindow, loggingWindow};

    Logging::linkLoggingWindow(loggingWindow.get());

    iniFile.read(iniStructure);
    for (auto& window : windows) {
        window->loadState(iniStructure);
    }

    AltimeterData data = {1, 2, 3};
    GCS_LOG_INFO("The following is AltimeterData - Altitude: {}, Value: {}, Timestamp: {}", data.altitude, data.status.value, data.timeStamp_ms);
}

void Application::menuItems() {
    if (ImGui::BeginMenu("Windows")) {
        for (const auto& window : windows) {
            ImGui::MenuItem(window->name, NULL, &window->visible);
        }
        ImGui::EndMenu();
    }
}

void Application::render() {
    // Already rendered from DockableWindow object, see if some code cleanup should be done in this area.
    // for (const auto& window : windows) {
    //    window->render();
    //}
}

void Application::shutdown() {
    for (auto& window : windows) {
        window->saveState(iniStructure);
        window = nullptr;
    }
    iniFile.write(iniStructure);

    Logging::unlinkLoggingWindow();

    ImPlot::DestroyContext();

    WSACleanup();
}

std::vector<HelloImGui::DockingSplit> Application::createBaseDockingSplits() {
    HelloImGui::DockingSplit splitPlotLogs;
    splitPlotLogs.initialDock = "MainDockSpace";
    splitPlotLogs.newDock = "LogSpace";
    splitPlotLogs.direction = ImGuiDir_Down;
    splitPlotLogs.ratio = 0.33f;

    HelloImGui::DockingSplit splitPlotMap;
    splitPlotMap.initialDock = "MainDockSpace";
    splitPlotMap.newDock = "MapSpace";
    splitPlotMap.direction = ImGuiDir_Left;
    splitPlotMap.ratio = 0.5f;

    std::vector<HelloImGui::DockingSplit> splits = {splitPlotLogs, splitPlotMap};
    return splits;
}

std::vector<HelloImGui::DockableWindow> Application::createDockableWindows() {
    std::vector<HelloImGui::DockableWindow> dockableWindows = {plotWindow->getDockableWindowObject("MainDockSpace"),
                                                               mapWindow->getDockableWindowObject("MapSpace"),
                                                               loggingWindow->getDockableWindowObject("LogSpace")};
    return dockableWindows;
}
