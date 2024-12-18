#include "Application.h"
#include "Constants.h"

#include <hello_imgui/hello_imgui.h>

int main(int, char*[]) {
    HelloImGui::RunnerParams runnerParams;

    runnerParams.appWindowParams.windowTitle = Constants::HELLO_IMGUI_WINDOW_TITLE;
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

    runnerParams.imGuiWindowParams.menuAppTitle = Constants::HELLO_IMGUI_MENU_APP_TITLE;
    runnerParams.imGuiWindowParams.showMenuBar = true;
    runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;

    runnerParams.iniFilename = Constants::HELLO_IMGUI_INI_FILENAME;

    HelloImGui::Run(runnerParams);
}
