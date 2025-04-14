#include "Application.h"
#include "Constants.h"

#include <hello_imgui/hello_imgui.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int, char*[]) {
    Application::init();

    HelloImGui::RunnerParams runnerParams;

    runnerParams.appWindowParams.windowTitle = Constants::HELLO_IMGUI_WINDOW_TITLE;
    runnerParams.appWindowParams.restorePreviousGeometry = true;

    runnerParams.appWindowParams.borderless = false;
    runnerParams.appWindowParams.borderlessMovable = true;
    runnerParams.appWindowParams.borderlessResizable = true;
    runnerParams.appWindowParams.borderlessClosable = true;

    runnerParams.dockingParams.dockingSplits = Application::createBaseDockingSplits();
    runnerParams.dockingParams.dockableWindows = Application::createDockableWindows();

    runnerParams.callbacks.LoadAdditionalFonts = Application::loadFonts;
    runnerParams.callbacks.PreNewFrame = Application::preNewFrame;
    runnerParams.callbacks.BeforeExit = Application::shutdown;

    runnerParams.imGuiWindowParams.menuAppTitle = Constants::HELLO_IMGUI_MENU_APP_TITLE;
    runnerParams.imGuiWindowParams.showMenuBar = true;
    runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;

    runnerParams.iniFilename = Constants::HELLO_IMGUI_INI_FILENAME;

    HelloImGui::Run(runnerParams);
}
