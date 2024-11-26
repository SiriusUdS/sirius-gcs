#include "ImGuiManager.h"

#include "Altimeter/AltimeterData.h"
#include "Logging.h"

#include <imgui.h>
#include <implot.h>
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

namespace ImGuiManager {
std::unique_ptr<ControlsWindow> controlsWindow;
std::unique_ptr<LoggingWindow> loggingWindow;
} // namespace ImGuiManager

void ImGuiManager::init() {
    Logging::initSpdLog();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        GCS_LOG_ERROR("WSAStartup failed");
        return; // Handle error here (add logs)
    }

    AltimeterData data = {1, 2, 3};
    GCS_LOG_INFO("The following is AltimeterData:");
    GCS_LOG_INFO(data.altitude);
    GCS_LOG_INFO(data.status.value);
    GCS_LOG_INFO(data.timeStamp_ms);

    ImPlot::CreateContext();

    controlsWindow = std::make_unique<ControlsWindow>();
    loggingWindow = std::make_unique<LoggingWindow>();

    Logging::linkLoggingWindow(loggingWindow.get());
}

void ImGuiManager::render() {
    controlsWindow->draw();
    loggingWindow->draw();
}

void ImGuiManager::shutdown() {
    controlsWindow = nullptr;
    loggingWindow = nullptr;

    Logging::removeLoggingWindow();

    ImPlot::DestroyContext();

    WSACleanup();
}