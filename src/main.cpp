#include "Application.h"

#include <hello_imgui/hello_imgui.h>

int main(int, char*[]) {
    HelloImGui::RunnerParams runnerParams;

    runnerParams.appWindowParams.windowTitle = "Sirius GCS";
    runnerParams.appWindowParams.windowGeometry.size = {800, 600};
    runnerParams.appWindowParams.restorePreviousGeometry = true;

    runnerParams.appWindowParams.borderless = false;
    runnerParams.appWindowParams.borderlessMovable = true;
    runnerParams.appWindowParams.borderlessResizable = true;
    runnerParams.appWindowParams.borderlessClosable = true;

    runnerParams.callbacks.LoadAdditionalFonts = Application::loadFonts;
    runnerParams.callbacks.PostInit = Application::init;
    runnerParams.callbacks.ShowMenus = Application::menuItems;
    runnerParams.callbacks.ShowGui = Application::render;
    runnerParams.callbacks.BeforeExit = Application::shutdown;

    runnerParams.imGuiWindowParams.menuAppTitle = "Sirius GCS";
    runnerParams.imGuiWindowParams.showMenuBar = true;
    runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;

    runnerParams.iniFilename = "hello_imgui.ini";

    HelloImGui::Run(runnerParams);
}
