#include "Application.h"

// clang-format off
#include <WinSock2.h>
// clang-format on

#include "ControlsWindow.h"
#include "Logging.h"
#include "LoggingWindow.h"
#include "MapWindow.h"
#include "PacketProcessing.h"
#include "PlotWindowCenter.h"
#include "SerialControl.h"
#include "SwitchesWindow.h"

#include <SerialTask.h>
#include <imgui.h>
#include <implot.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

namespace Application {
mINI::INIFile iniFile(Constants::GCS_INI_FILENAME);
mINI::INIStructure iniStructure;
} // namespace Application

void Application::loadFonts() {
    static constexpr ImWchar ICONS_RANGES[] = {0xf000, 0xf8ff, 0};

    constexpr const char* MAIN_FONT = "assets/fonts/Nunito-Regular.ttf";
    constexpr const char* ICONS_FONT = "assets/fonts/fa-solid-900.ttf";
    constexpr float MAIN_FONT_SIZE = 28;
    constexpr float ICONS_SIZE = 22;

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(MAIN_FONT, MAIN_FONT_SIZE);

    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;

    io.Fonts->AddFontFromFileTTF(ICONS_FONT, ICONS_SIZE, &iconsConfig, ICONS_RANGES);
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
