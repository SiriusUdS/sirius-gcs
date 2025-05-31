#include "Application.h"

#include "Constants.h"
#include "ControlsWindow.h"
#include "Logging.h"
#include "LoggingWindow.h"
#include "MapWindow.h"
#include "PacketProcessing.h"
#include "PlotWindowCenter.h"
#include "SerialControl.h"
#include "SwitchesWindow.h"

#include <SerialTask.h>
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

    MapWindow::init();

    LoggingWindow::loadState(iniStructure);
    MapWindow::loadState(iniStructure);
    PlotWindowCenter::loadState(iniStructure);

    SerialTask::start();
}

void Application::preNewFrame() {
}

void Application::shutdown() {
    SerialTask::stop();

    LoggingWindow::saveState(iniStructure);
    MapWindow::saveState(iniStructure);
    PlotWindowCenter::saveState(iniStructure);

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
    HelloImGui::DockableWindow loggingDockWin(Constants::GCS_LOGGING_WINDOW_ID, Constants::GCS_LOGGING_DOCKSPACE, []() { LoggingWindow::render(); });
    HelloImGui::DockableWindow mapDockWin(Constants::GCS_MAP_WINDOW_ID, Constants::GCS_MAP_DOCKSPACE, []() { MapWindow::render(); });
    HelloImGui::DockableWindow controlsDockWin(Constants::GCS_CONTROLS_WINDOW_ID, Constants::GCS_PLOT_DOCKSPACE, []() { ControlsWindow::render(); });
    HelloImGui::DockableWindow switchesDockWin(Constants::GCS_SWITCHES_WINDOW_ID, Constants::GCS_PLOT_DOCKSPACE, []() { SwitchesWindow::render(); });

    std::vector<HelloImGui::DockableWindow> dockableWindows = PlotWindowCenter::createDockableWindows();
    dockableWindows.push_back(loggingDockWin);
    dockableWindows.push_back(mapDockWin);
    dockableWindows.push_back(controlsDockWin);
    dockableWindows.push_back(switchesDockWin);

    return dockableWindows;
}
