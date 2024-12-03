#include "LoggingWindow.h"

#include "Logging.h"

LoggingWindow::LoggingWindow() : Window("Logs"), _autoScroll(true) {
    clear();
}

void LoggingWindow::renderContent() {
    ImGui::Checkbox("Auto Scroll", &_autoScroll);
    ImGui::SameLine();
    if (ImGui::Button("Add test log")) {
        GCS_LOG_INFO("This is a test");
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        clear();
    }
    ImGui::SameLine();
    _filter.Draw("Filter", -100.0f);
    ImGui::Separator();
    if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        const char* bufBegin = _buf.begin();
        const char* bufEnd = _buf.end();
        if (_filter.IsActive()) {
            for (int lineNo = 0; lineNo < _lineOffsets.size(); lineNo++) {
                const char* lineStart = bufBegin + _lineOffsets[lineNo];
                const char* lineEnd = (lineNo + 1 < _lineOffsets.size()) ? (bufBegin + _lineOffsets[lineNo + 1] - 1) : bufEnd;
                if (_filter.PassFilter(lineStart, lineEnd)) {
                    ImGui::TextUnformatted(lineStart, lineEnd);
                }
            }
        } else {
            ImGui::TextUnformatted(bufBegin, bufEnd);
        }

        if (_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::LogText("Bonjour");
}

void LoggingWindow::clear() {
    _buf.clear();
    _lineOffsets.clear();
    _lineOffsets.push_back(0);
}

void LoggingWindow::addLog(const char* str, const char* strEnd) {
    int oldSize = _buf.size();
    _buf.append(str, strEnd);
    for (const int newSize = _buf.size(); oldSize < newSize; oldSize++) {
        if (_buf[oldSize] == '\n') {
            _lineOffsets.push_back(oldSize + 1);
        }
    }
}
