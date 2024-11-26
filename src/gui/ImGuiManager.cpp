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

    ImPlot::CreateContext();

    controlsWindow = std::make_unique<ControlsWindow>();
    loggingWindow = std::make_unique<LoggingWindow>();

    Logging::linkLoggingWindow(loggingWindow.get());

    AltimeterData data = {1, 2, 3};
    GCS_LOG_INFO(
        "The following is AltimeterData - Altitude: {}, Valeur: {}, Timestamp: {}",
        data.altitude,
        data.status.value,
        data.timeStamp_ms
    );
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