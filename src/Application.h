#ifndef APPLICATION_H
#define APPLICATION_H

#include <chrono>
#include <hello_imgui/hello_imgui.h>
#include <ini.h>

namespace Application {
void loadFonts();
void init();
void preNewFrame();
void shutdown();

std::vector<HelloImGui::DockingSplit> createBaseDockingSplits();
std::vector<HelloImGui::DockableWindow> createDockableWindows();

extern mINI::INIFile iniFile;
extern mINI::INIStructure iniStructure;
} // namespace Application

#endif // APPLICATION_H
