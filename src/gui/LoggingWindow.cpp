#include "LoggingWindow.h"

#include "Constants.h"
#include "Logging.h"

LoggingWindow::LoggingWindow() : Window(Constants::GCS_LOGGING_WINDOW_ID), autoScroll(true) {
    clear();
}

void LoggingWindow::renderContent() {
    ImGui::Checkbox("Auto Scroll", &autoScroll);
    ImGui::SameLine();
    if (ImGui::Button("Add test log")) {
        GCS_LOG_INFO("This is a test");
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        clear();
    }
    ImGui::SameLine();
    filter.Draw("Filter", -100.0f);
    ImGui::Separator();
    if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        const char* bufBegin = buf.begin();
        const char* bufEnd = buf.end();
        if (filter.IsActive()) {
            for (int lineNo = 0; lineNo < lineOffsets.size(); lineNo++) {
                const char* lineStart = bufBegin + lineOffsets[lineNo];
                const char* lineEnd = (lineNo + 1 < lineOffsets.size()) ? (bufBegin + lineOffsets[lineNo + 1] - 1) : bufEnd;
                if (filter.PassFilter(lineStart, lineEnd)) {
                    ImGui::TextUnformatted(lineStart, lineEnd);
                }
            }
        } else {
            ImGui::TextUnformatted(bufBegin, bufEnd);
        }

        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::LogText("Bonjour");
}

void LoggingWindow::clear() {
    buf.clear();
    lineOffsets.clear();
    lineOffsets.push_back(0);
}

void LoggingWindow::addLog(const char* str, const char* strEnd) {
    int oldSize = buf.size();
    buf.append(str, strEnd);
    for (const int newSize = buf.size(); oldSize < newSize; oldSize++) {
        if (buf[oldSize] == '\n') {
            lineOffsets.push_back(oldSize + 1);
        }
    }
}
