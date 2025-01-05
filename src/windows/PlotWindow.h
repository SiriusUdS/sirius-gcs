#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "PlotData.h"
#include "RichMapPlot.h"
#include "RichMarkStorage.h"

namespace PlotWindow {
void render();
void loadState(const mINI::INIStructure& ini);
void saveState(mINI::INIStructure& ini);

extern ImPlotFlags flags;
extern bool autofit;
extern std::vector<PlotData> data;
} // namespace PlotWindow

#endif // PLOTWINDOW_H
