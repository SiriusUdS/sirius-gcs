#include "Application.h"

// clang-format off
#include <WinSock2.h>
// clang-format on

#include "ControlsWindow.h"
#include "FontConfig.h"
#include "ImGuiConfig.h"
#include "Logging.h"
#include "LoggingWindow.h"
#include "MapWindow.h"
#include "PlotWindowCenter.h"
#include "SerialComWindow.h"
#include "SerialTask.h"
#include "SwitchesWindow.h"

#include <imgui.h>
#include <implot.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

namespace Application {
mINI::INIFile iniFile("sirius_gcs.ini");
mINI::INIStructure iniStructure;
} // namespace Application

void Application::loadFonts() {
    static constexpr ImWchar ICONS_RANGES[] = {0xf000, 0xf8ff, 0};
    static constexpr const char* MAIN_FONT = "assets/fonts/Nunito-Regular.ttf";
    static constexpr const char* BOLD_MAIN_FONT = "assets/fonts/Nunito-Bold.ttf";
    static constexpr const char* ICONS_FONT = "assets/fonts/fa-solid-900.ttf";
    static constexpr const char* CODE_FONT = "assets/fonts/Consolas-Regular.ttf";
    static constexpr float MAIN_FONT_SIZE = 24.f;
    static constexpr float SMALLER_MAIN_FONT_SIZE = 20.f;
    static constexpr float ICONS_SIZE = 20.f;
    static constexpr float CODE_SIZE = 20.f;

    ImGuiIO& io = ImGui::GetIO();

    FontConfig::defaultFont = io.Fonts->AddFontFromFileTTF(MAIN_FONT, MAIN_FONT_SIZE);

    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF(ICONS_FONT, ICONS_SIZE, &iconsConfig, ICONS_RANGES);

    io.FontDefault = FontConfig::defaultFont;

    // All other fonts need to be added below this comment to avoid interference with merge between main font and icons font
    FontConfig::boldDefaultFont = io.Fonts->AddFontFromFileTTF(BOLD_MAIN_FONT, MAIN_FONT_SIZE);
    FontConfig::smallerDefaultFont = io.Fonts->AddFontFromFileTTF(MAIN_FONT, SMALLER_MAIN_FONT_SIZE);
    FontConfig::codeFont = io.Fonts->AddFontFromFileTTF(CODE_FONT, CODE_SIZE);
}

void Application::init() {
    Logging::init();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        GCS_LOG_ERROR("WSAStartup failed.");
        return;
    }

    ImPlot::CreateContext();

    iniFile.read(iniStructure);

    LoggingWindow::init();
    MapWindow::init();

    LoggingWindow::loadState(iniStructure);
    MapWindow::loadState(iniStructure);
    PlotWindowCenter::loadState(iniStructure);
    SerialComWindow::loadState(iniStructure);

    SerialTask::start();
}

void Application::preNewFrame() {
}

void Application::shutdown() {
    SerialTask::stop();

    LoggingWindow::saveState(iniStructure);
    MapWindow::saveState(iniStructure);
    PlotWindowCenter::saveState(iniStructure);
    SerialComWindow::saveState(iniStructure);

    iniFile.write(iniStructure);

    ImPlot::DestroyContext();

    WSACleanup();
}

std::vector<HelloImGui::DockingSplit> Application::createBaseDockingSplits() {
    HelloImGui::DockingSplit splitPlotLogs;
    splitPlotLogs.initialDock = ImGuiConfig::Dockspace::PLOT;
    splitPlotLogs.newDock = ImGuiConfig::Dockspace::LOGGING;
    splitPlotLogs.direction = ImGuiDir_Down;
    splitPlotLogs.ratio = 0.33f;

    HelloImGui::DockingSplit splitPlotMap;
    splitPlotMap.initialDock = ImGuiConfig::Dockspace::PLOT;
    splitPlotMap.newDock = ImGuiConfig::Dockspace::MAP;
    splitPlotMap.direction = ImGuiDir_Left;
    splitPlotMap.ratio = 0.5f;

    std::vector<HelloImGui::DockingSplit> splits = {splitPlotLogs, splitPlotMap};
    return splits;
}

std::vector<HelloImGui::DockableWindow> Application::createDockableWindows() {
    HelloImGui::DockableWindow loggingDockWin("Logs", ImGuiConfig::Dockspace::LOGGING, []() { LoggingWindow::render(); });
    HelloImGui::DockableWindow mapDockWin("Map", ImGuiConfig::Dockspace::MAP, []() { MapWindow::render(); });
    HelloImGui::DockableWindow controlsDockWin("Controls", ImGuiConfig::Dockspace::MAP, []() { ControlsWindow::render(); });
    HelloImGui::DockableWindow serialComDockWin("Serial COM", ImGuiConfig::Dockspace::MAP, []() { SerialComWindow::render(); });
    HelloImGui::DockableWindow switchesDockWin("Switches", ImGuiConfig::Dockspace::MAP, []() { SwitchesWindow::render(); });

    std::vector<HelloImGui::DockableWindow> dockableWindows = PlotWindowCenter::createDockableWindows();
    dockableWindows.push_back(loggingDockWin);
    dockableWindows.push_back(mapDockWin);
    dockableWindows.push_back(controlsDockWin);
    dockableWindows.push_back(serialComDockWin);
    dockableWindows.push_back(switchesDockWin);

    return dockableWindows;
}
