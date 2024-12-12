#include "PlotWindow.h"

#include "Logging.h"
#include "PlotColors.h"

#include <imgui.h>
#include <implot.h>

PlotWindow::PlotWindow() : Window("Plot") {
    PlotData d1;
    d1.addData(0.0f, 0.0f);
    d1.addData(1.0f, 10.0f);
    d1.addData(2.0f, 7.5f);
    d1.addData(3.0f, 12.5f);
    d1.addData(4.0f, 15.0f);
    d1.setName("Data 1");
    d1.setColor(PlotColors::YELLOW);

    PlotData d2;
    d2.addData(0.0f, 0.0f);
    d2.addData(1.0f, 5.0f);
    d2.addData(2.0f, 12.5f);
    d2.addData(3.0f, 5.0f);
    d2.addData(4.0f, 10.0f);
    d2.setName("Data 2");
    d2.setColor(PlotColors::PURPLE);

    data.push_back(d1);
    data.push_back(d2);
}

void PlotWindow::renderContent() {
    if (ImPlot::BeginPlot("Line Plot", ImGui::GetContentRegionAvail())) {
        for (const auto& plotData : data) {
            plotData.plot();
        }
        ImPlot::EndPlot();
    }
}
