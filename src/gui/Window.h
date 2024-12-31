#ifndef WINDOW_H
#define WINDOW_H

#include <hello_imgui/hello_imgui.h>
#include <ini.h>
#include <string>

class Window {
public:
    Window();
    void render();
    virtual void loadState(const mINI::INIStructure& ini);
    virtual void saveState(mINI::INIStructure& ini) const;
    virtual void renderContent() = 0;

    std::string getName();
    bool& isVisible();

    size_t dockableWindowIndex{};
};

#endif // WINDOW_H
