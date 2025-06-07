#include "LoggingWindow.h"

#include "Constants.h"
#include "Logging.h"
#include "ToggleButton.h"

#include <ini.h>

namespace LoggingWindow {
bool autoScroll{true}, showInfo{true}, showWarn{true}, showError{true}, showTrace{true}, showDebug{true};
ImGuiTextBuffer buf;
ImGuiTextFilter filter;
ImVector<int> lineOffsets;
ImVector<spdlog::level::level_enum> logLevels;
} // namespace LoggingWindow

void LoggingWindow::render() {
    ImGui::Checkbox("Auto Scroll", &autoScroll);
    ImGui::SameLine();
    ToggleButton("Debug", &showDebug, ImVec4(0.0f, 0.5f, 1.0f, 1.0f));
    ImGui::SameLine();
    ToggleButton("Info", &showInfo, ImVec4(0.0f, 0.6f, 0.15f, 1.0f));
    ImGui::SameLine();
    ToggleButton("Warn", &showWarn, ImVec4(0.7f, 0.55f, 0.0f, 1.0f));
    ImGui::SameLine();
    ToggleButton("Error", &showError, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
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

        for (int lineNo = 0; lineNo < lineOffsets.size(); lineNo++) {
            const char* lineStart = bufBegin + lineOffsets[lineNo];
            const char* lineEnd = (lineNo + 1 < lineOffsets.size()) ? (bufBegin + lineOffsets[lineNo + 1] - 1) : bufEnd;

            if (lineEnd != bufEnd) {
                bool show = false;
                // clang-format off
                switch (logLevels[lineNo]) {
                    case spdlog::level::debug: show = showDebug; break;
                    case spdlog::level::info:  show = showInfo;  break;
                    case spdlog::level::warn:  show = showWarn;  break;
                    case spdlog::level::err:   show = showError; break;
                    default:                   show = true;      break;
                }
                // clang-format on
                if (!show) {
                    continue;
                }
            }

            if (filter.IsActive() && !filter.PassFilter(lineStart, lineEnd)) {
                continue;
            }

            ImGui::TextUnformatted(lineStart, lineEnd);
        }

        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
}

void LoggingWindow::loadState(const mINI::INIStructure& ini) {
    clear();

    if (ini.has(Constants::GCS_INI_SECTION)) {
        if (ini.get(Constants::GCS_INI_SECTION).has(Constants::GCS_INI_LOG_WINDOW_AUTO_SCROLL)) {
            autoScroll = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(Constants::GCS_INI_LOG_WINDOW_AUTO_SCROLL));
        }
        if (ini.get(Constants::GCS_INI_SECTION).has(Constants::GCS_INI_LOG_WINDOW_SHOW_DEBUG)) {
            showDebug = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(Constants::GCS_INI_LOG_WINDOW_SHOW_DEBUG));
        }
        if (ini.get(Constants::GCS_INI_SECTION).has(Constants::GCS_INI_LOG_WINDOW_SHOW_INFO)) {
            showInfo = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(Constants::GCS_INI_LOG_WINDOW_SHOW_INFO));
        }
        if (ini.get(Constants::GCS_INI_SECTION).has(Constants::GCS_INI_LOG_WINDOW_SHOW_WARN)) {
            showWarn = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(Constants::GCS_INI_LOG_WINDOW_SHOW_WARN));
        }
        if (ini.get(Constants::GCS_INI_SECTION).has(Constants::GCS_INI_LOG_WINDOW_SHOW_ERROR)) {
            showError = std::stoi(ini.get(Constants::GCS_INI_SECTION).get(Constants::GCS_INI_LOG_WINDOW_SHOW_ERROR));
        }
    }
}

void LoggingWindow::saveState(mINI::INIStructure& ini) {
    ini[Constants::GCS_INI_SECTION].set(Constants::GCS_INI_LOG_WINDOW_AUTO_SCROLL, std::to_string(autoScroll));
    ini[Constants::GCS_INI_SECTION].set(Constants::GCS_INI_LOG_WINDOW_SHOW_DEBUG, std::to_string(showDebug));
    ini[Constants::GCS_INI_SECTION].set(Constants::GCS_INI_LOG_WINDOW_SHOW_INFO, std::to_string(showInfo));
    ini[Constants::GCS_INI_SECTION].set(Constants::GCS_INI_LOG_WINDOW_SHOW_WARN, std::to_string(showWarn));
    ini[Constants::GCS_INI_SECTION].set(Constants::GCS_INI_LOG_WINDOW_SHOW_ERROR, std::to_string(showError));
}

void LoggingWindow::clear() {
    buf.clear();
    lineOffsets.clear();
    lineOffsets.push_back(0);
}

void LoggingWindow::addLog(const char* str, const char* strEnd, spdlog::level::level_enum type) {
    int oldSize = buf.size();
    buf.append(str, strEnd);
    for (const int newSize = buf.size(); oldSize < newSize; oldSize++) {
        if (buf[oldSize] == '\n') {
            lineOffsets.push_back(oldSize + 1);
            logLevels.push_back(type);
        }
    }
}
