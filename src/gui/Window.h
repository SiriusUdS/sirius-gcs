#ifndef WINDOW_H
#define WINDOW_H

#include <ini.h>
#include <string>

class Window {
public:
    Window(const char* name);
    void render();
    virtual void loadState(const mINI::INIStructure& ini);
    virtual void saveState(mINI::INIStructure& ini) const;
    virtual void renderContent() = 0;

    bool visible;
    const char* name;
};

#endif // WINDOW_H
