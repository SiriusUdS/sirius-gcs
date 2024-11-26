#include "Altimeter/AltimeterData.h"
#include "Application.h"
#include "Logging.h"
#include "hello_imgui/hello_imgui.h"

#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

int main(int, char*[]) {
    Logging::initSpdLog();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        GCS_LOG_ERROR("WSAStartup failed");
        return -1;
    }

    AltimeterData data = {1, 2, 3};
    GCS_LOG_INFO("The following is AltimeterData:");
    GCS_LOG_INFO(data.altitude);
    GCS_LOG_INFO(data.status.value);
    GCS_LOG_INFO(data.timeStamp_ms);

    Application::init();

    HelloImGui::RunnerParams runnerParams;

    runnerParams.appWindowParams.windowTitle = "Sirius GCS";
    runnerParams.imGuiWindowParams.menuAppTitle = "Sirius GCS";
    runnerParams.appWindowParams.windowGeometry.size = {1920, 1080};
    runnerParams.appWindowParams.restorePreviousGeometry = true;

    runnerParams.appWindowParams.borderless = true;
    runnerParams.appWindowParams.borderlessMovable = true;
    runnerParams.appWindowParams.borderlessResizable = true;
    runnerParams.appWindowParams.borderlessClosable = true;

    runnerParams.imGuiWindowParams.showMenuBar = true;

    runnerParams.callbacks.ShowGui = Application::render;

    runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;

    HelloImGui::Run(runnerParams);

    Application::shutdown();

    WSACleanup();
}
