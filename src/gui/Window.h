#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class Window {
public:
    Window(const char* name);
    void render();
    virtual void renderContent() = 0;

    bool visible;
    const char* name;
};

#endif // WINDOW_H
