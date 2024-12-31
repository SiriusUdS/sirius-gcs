#ifndef LEDWINDOW_H
#define LEDWINDOW_H

#include "Window.h"

class LedWindow : public Window {
public:
    explicit LedWindow();
    void renderContent() override;

private:
    bool open{};
};

#endif // LEDWINDOW_H
