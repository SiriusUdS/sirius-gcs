#include "Application.h"

#include "Constants.h"
#include "ControlsWindow.h"
#include "LedWindow.h"
#include "Logging.h"
#include "LoggingWindow.h"
#include "MapWindow.h"
#include "PacketProcessing.h"
#include "PlotWindow.h"

#include <WinSock2.h>
#include <imgui.h>
#include <implot.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

namespace Application {
mINI::INIFile iniFile(Constants::GCS_INI_FILENAME);
mINI::INIStructure iniStructure;
SerialCom serialCom;
std::chrono::time_point<std::chrono::steady_clock> lastSerialReadTime = std::chrono::steady_clock::now();
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

    ControlsWindow::init();
    MapWindow::init();

    LoggingWindow::loadState(iniStructure);
    MapWindow::loadState(iniStructure);
    PlotWindow::loadState(iniStructure);
}

void Application::preNewFrame() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - lastSerialReadTime;
    lastSerialReadTime = now;

    size_t bytesToRead = Constants::RECV_BYTES_PER_SECOND * elapsed.count();
    while (0 < bytesToRead--) {
        Application::serialCom.read();
    }

    PacketProcessing::processIncomingPacket();
}

void Application::shutdown() {
    LoggingWindow::saveState(iniStructure);
    MapWindow::saveState(iniStructure);
    PlotWindow::saveState(iniStructure);

    ControlsWindow::shutdown();

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
    HelloImGui::DockableWindow controlsDockWin(Constants::GCS_CONTROLS_WINDOW_ID, Constants::GCS_PLOT_DOCKSPACE, []() { ControlsWindow::render(); });

    return {ledDockWin, loggingDockWin, mapDockWin, plotDockWin, controlsDockWin};
}
