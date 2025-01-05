#include "Application.h"

#include "Altimeter/AltimeterData.h"
#include "Constants.h"
#include "LedWindow.h"
#include "Logging.h"
#include "LoggingWindow.h"
#include "MapWindow.h"
#include "PlotWindow.h"

#include <WinSock2.h>
#include <imgui.h>
#include <implot.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

namespace Application {
mINI::INIFile iniFile(Constants::GCS_INI_FILENAME);
mINI::INIStructure iniStructure;
} // namespace Application

void Application::loadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(Constants::IMGUI_MAIN_FONT_FILENAME, Constants::IMGUI_MAIN_FONT_SIZE);
}

void Application::init() {
    Logging::init();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        GCS_LOG_ERROR("WSAStartup failed");
        return;
    }

    ImPlot::CreateContext();

    iniFile.read(iniStructure);

    LoggingWindow::init();
    MapWindow::loadState(iniStructure);
    PlotWindow::loadState(iniStructure);

    AltimeterData data = {1, 2, 3};
    GCS_LOG_INFO("The following is AltimeterData - Altitude: {}, Value: {}, Timestamp: {}", data.altitude, data.status.value, data.timeStamp_ms);
}

void Application::shutdown() {
    MapWindow::saveState(iniStructure);
    PlotWindow::saveState(iniStructure);
    iniFile.write(iniStructure);

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
    HelloImGui::DockableWindow ledDockWin(Constants::GCS_LED_WINDOW_ID, Constants::GCS_LED_DOCKSPACE, []() { LedWindow::render(); });
    HelloImGui::DockableWindow loggingDockWin(Constants::GCS_LOGGING_WINDOW_ID, Constants::GCS_LOGGING_DOCKSPACE, []() { LoggingWindow::render(); });
    HelloImGui::DockableWindow mapDockWin(Constants::GCS_MAP_WINDOW_ID, Constants::GCS_MAP_DOCKSPACE, []() { MapWindow::render(); });
    HelloImGui::DockableWindow plotDockWin(Constants::GCS_PLOT_WINDOW_ID, Constants::GCS_PLOT_DOCKSPACE, []() { PlotWindow::render(); });

    return {ledDockWin, loggingDockWin, mapDockWin, plotDockWin};
}
