#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "PlotData.h"
#include "RichMapPlot.h"
#include "RichMarkStorage.h"
#include "Window.h"

class PlotWindow : public Window {
public:
    PlotWindow();
    void renderContent();

private:
    std::vector<PlotData> data;
    ImPlotFlags flags;
    bool autofit{};
};

#endif // PLOTWINDOW_H
