#ifndef WINDOW_H
#define WINDOW_H

#include <ini.h>

class Window {
public:
    virtual void render() = 0;
    virtual void loadState(const mINI::INIStructure& ini) {
    }
    virtual void saveState(mINI::INIStructure& ini) const {
    }
};

#endif // WINDOW_H
