#include "Application.h"

// clang-format off
#include <WinSock2.h>
// clang-format on

#include "BoardsWindow.h"
#include "ControlsWindow.h"
#include "FontConfig.h"
#include "ImGuiConfig.h"
#include "Logging.h"
#include "LoggingWindow.h"
#include "MapWindow.h"
#include "NOSPhaseDiagramWindow.h"
#include "PacketCSVLogging.h"
#include "PlotWindowCenter.h"
#include "SerialComWindow.h"
#include "SerialTask.h"
#include "SwitchesWindow.h"
#include "ValvesWindow.h"

#include <imgui.h>
#include <implot.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

namespace Application {
mINI::INIFile iniFile("sirius_gcs.ini");
mINI::INIStructure iniStructure;
} // namespace Application

void Application::loadFonts() {
    const std::string mainFontPath = "fonts/Nunito-Regular.ttf";
    const std::string boldMainFontPath = "fonts/Nunito-Bold.ttf";
    const std::string monospaceFontPath = "fonts/Consolas-Regular.ttf";

    FontConfig::mainFont = HelloImGui::LoadFont(mainFontPath, 24.f);
    HelloImGui::MergeFontAwesomeToLastFont(20.f);
    FontConfig::boldMainFont = HelloImGui::LoadFont(boldMainFontPath, 24.f);
    FontConfig::monospaceFont = HelloImGui::LoadFont(monospaceFontPath, 20.f);
}

void Application::init() {
    Logging::init();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        GCS_APP_LOG_ERROR("WSAStartup failed.");
        return;
    }

    ImPlot::CreateContext();

    iniFile.read(iniStructure);

    MapWindow::init();
    NOSPhaseDiagramWindow::init();
    PacketCSVLogging::init();

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
    HelloImGui::DockableWindow boardsDockWin("Boards", ImGuiConfig::Dockspace::MAP, []() { BoardsWindow::render(); });
    HelloImGui::DockableWindow controlsDockWin("Controls", ImGuiConfig::Dockspace::MAP, []() { ControlsWindow::render(); });
    HelloImGui::DockableWindow loggingDockWin("Logs", ImGuiConfig::Dockspace::LOGGING, []() { LoggingWindow::render(); });
    HelloImGui::DockableWindow mapDockWin("Map", ImGuiConfig::Dockspace::MAP, []() { MapWindow::render(); });
    HelloImGui::DockableWindow nosPhaseDiagramDockWin("NOS Phase Diagram", ImGuiConfig::Dockspace::PLOT, []() { NOSPhaseDiagramWindow::render(); });
    HelloImGui::DockableWindow serialComDockWin("Serial COM", ImGuiConfig::Dockspace::MAP, []() { SerialComWindow::render(); });
    HelloImGui::DockableWindow switchesDockWin("Switches", ImGuiConfig::Dockspace::MAP, []() { SwitchesWindow::render(); });
    HelloImGui::DockableWindow valvesDockWin("Valves", ImGuiConfig::Dockspace::MAP, []() { ValvesWindow::render(); });

    std::vector<HelloImGui::DockableWindow> dockableWindows = PlotWindowCenter::createDockableWindows();
    dockableWindows.push_back(boardsDockWin);
    dockableWindows.push_back(controlsDockWin);
    dockableWindows.push_back(loggingDockWin);
    dockableWindows.push_back(mapDockWin);
    dockableWindows.push_back(nosPhaseDiagramDockWin);
    dockableWindows.push_back(serialComDockWin);
    dockableWindows.push_back(switchesDockWin);
    dockableWindows.push_back(valvesDockWin);

    return dockableWindows;
}
