#include "Application.h"

#include "Altimeter/AltimeterData.h"
#include "Logging.h"

#include <WinSock2.h>
#include <imgui.h>
#include <hello_imgui/hello_imgui.h>
#include <implot.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

namespace Application {
std::unique_ptr<MapWindow> mapWindow;
std::unique_ptr<PlotWindow> plotWindow;
std::unique_ptr<LoggingWindow> loggingWindow;
} // namespace Application

void Application::loadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("assets/fonts/Comic Sans MS.ttf", 28.f);
}

void Application::init() {
    Logging::initSpdLog();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        GCS_LOG_ERROR("WSAStartup failed");
        return; // Handle error here (add logs)
    }

    ImPlot::CreateContext();

    mapWindow = std::make_unique<MapWindow>();
    plotWindow = std::make_unique<PlotWindow>();
    loggingWindow = std::make_unique<LoggingWindow>();

    Logging::linkLoggingWindow(loggingWindow.get());

    AltimeterData data = {1, 2, 3};
    GCS_LOG_INFO("The following is AltimeterData - Altitude: {}, Value: {}, Timestamp: {}", data.altitude, data.status.value, data.timeStamp_ms);
}

void Application::menuItems() {
    if (ImGui::BeginMenu("Windows")) {
        // TODO - Do this dynamically
        ImGui::MenuItem(loggingWindow->name, NULL, &loggingWindow->visible);
        ImGui::MenuItem(plotWindow->name, NULL, &plotWindow->visible);
        ImGui::MenuItem(mapWindow->name, NULL, &mapWindow->visible);
    
        ImGui::EndMenu();
    }
}

void Application::render() {
    mapWindow->render();
    plotWindow->render();
    loggingWindow->render();
}

void Application::shutdown() {
    mapWindow = nullptr;
    plotWindow = nullptr;
    loggingWindow = nullptr;

    Logging::removeLoggingWindow();

    ImPlot::DestroyContext();

    WSACleanup();
}