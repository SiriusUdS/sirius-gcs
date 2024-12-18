#include "Application.h"

#include "Altimeter/AltimeterData.h"
#include "Constants.h"
#include "Logging.h"

#include <WinSock2.h>
#include <hello_imgui/hello_imgui.h>
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
    for (const auto& window : windows) {
        window->render();
    }
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