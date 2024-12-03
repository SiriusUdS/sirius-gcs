#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "RichMapPlot.h"
#include "RichMarkStorage.h"
#include "Window.h"

class PlotWindow : public Window {
public:
    PlotWindow();
    void renderContent();
};

#endif // PLOTWINDOW_H
