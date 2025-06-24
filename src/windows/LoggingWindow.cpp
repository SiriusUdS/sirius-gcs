#include "LoggingWindow.h"

#include "FontAwesome.h"
#include "IniConfig.h"
#include "Logging.h"
#include "ToggleButton.h"

#include <ini.h>

namespace LoggingWindow {
constexpr const char* GCS_INI_LOG_WINDOW_AUTO_SCROLL = "log_window_auto_scroll";
constexpr const char* GCS_INI_LOG_WINDOW_SHOW_DEBUG = "log_window_show_debug";
constexpr const char* GCS_INI_LOG_WINDOW_SHOW_INFO = "log_window_show_info";
constexpr const char* GCS_INI_LOG_WINDOW_SHOW_WARN = "log_window_show_warn";
constexpr const char* GCS_INI_LOG_WINDOW_SHOW_ERROR = "log_window_show_error";

bool showInfo{true}, showWarn{true}, showError{true}, showTrace{true}, showDebug{true};
ImGuiTextBuffer buf;
ImGuiTextFilter filter;
ImVector<int> lineOffsets;
ImVector<spdlog::level::level_enum> logLevels;
ImVector<int> visibleLines;
} // namespace LoggingWindow

void LoggingWindow::init() {
    clear();
}

void LoggingWindow::render() {
    if (ToggleButton(ICON_FA_BUG " Debug", &showDebug, ImVec4(0.0f, 0.5f, 1.0f, 1.0f))) {
        updateVisibleLines();
    }
    ImGui::SameLine();
    if (ToggleButton(ICON_FA_INFO_CIRCLE " Info", &showInfo, ImVec4(0.0f, 0.6f, 0.15f, 1.0f))) {
        updateVisibleLines();
    }
    ImGui::SameLine();
    if (ToggleButton(ICON_FA_EXCLAMATION_TRIANGLE " Warn", &showWarn, ImVec4(0.7f, 0.55f, 0.0f, 1.0f))) {
        updateVisibleLines();
    }
    ImGui::SameLine();
    if (ToggleButton(ICON_FA_BOMB " Error", &showError, ImVec4(0.7f, 0.1f, 0.1f, 1.0f))) {
        updateVisibleLines();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        clear();
    }
    ImGui::SameLine();
    if (filter.Draw("Filter", -100.0f)) {
        updateVisibleLines();
    }

    ImGui::Separator();

    if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        const char* bufBegin = buf.begin();
        const char* bufEnd = buf.end();

        ImGuiListClipper clipper;
        clipper.Begin(visibleLines.size());
        while (clipper.Step()) {
            for (int lineNo = clipper.DisplayStart; lineNo < clipper.DisplayEnd; lineNo++) {
                const int lineIdx = visibleLines[lineNo];
                const char* lineStart = bufBegin + lineOffsets[lineIdx];
                const char* lineEnd = (lineIdx + 1 < lineOffsets.size()) ? (bufBegin + lineOffsets[lineIdx + 1]) : bufEnd;
                ImVec4 color(1.0f, 1.0f, 1.0f, 1.0f);

                // clang-format off
                switch (logLevels[lineIdx]) {
                    case spdlog::level::debug: color = ImVec4(0.2f, 0.6f, 1.0f, 1.0f); break;
                    case spdlog::level::info:  break;
                    case spdlog::level::warn:  color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); break;
                    case spdlog::level::err:   color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); break;
                    default:                   break;
                }
                // clang-format on

                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(lineStart, lineEnd);
                ImGui::PopStyleColor();
            }
        }
        clipper.End();

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
}

void LoggingWindow::loadState(const mINI::INIStructure& ini) {
    if (ini.has(IniConfig::GCS_SECTION)) {
        if (ini.get(IniConfig::GCS_SECTION).has(GCS_INI_LOG_WINDOW_SHOW_DEBUG)) {
            showDebug = std::stoi(ini.get(IniConfig::GCS_SECTION).get(GCS_INI_LOG_WINDOW_SHOW_DEBUG));
        }
        if (ini.get(IniConfig::GCS_SECTION).has(GCS_INI_LOG_WINDOW_SHOW_INFO)) {
            showInfo = std::stoi(ini.get(IniConfig::GCS_SECTION).get(GCS_INI_LOG_WINDOW_SHOW_INFO));
        }
        if (ini.get(IniConfig::GCS_SECTION).has(GCS_INI_LOG_WINDOW_SHOW_WARN)) {
            showWarn = std::stoi(ini.get(IniConfig::GCS_SECTION).get(GCS_INI_LOG_WINDOW_SHOW_WARN));
        }
        if (ini.get(IniConfig::GCS_SECTION).has(GCS_INI_LOG_WINDOW_SHOW_ERROR)) {
            showError = std::stoi(ini.get(IniConfig::GCS_SECTION).get(GCS_INI_LOG_WINDOW_SHOW_ERROR));
        }
    }
}

void LoggingWindow::saveState(mINI::INIStructure& ini) {
    ini[IniConfig::GCS_SECTION].set(GCS_INI_LOG_WINDOW_SHOW_DEBUG, std::to_string(showDebug));
    ini[IniConfig::GCS_SECTION].set(GCS_INI_LOG_WINDOW_SHOW_INFO, std::to_string(showInfo));
    ini[IniConfig::GCS_SECTION].set(GCS_INI_LOG_WINDOW_SHOW_WARN, std::to_string(showWarn));
    ini[IniConfig::GCS_SECTION].set(GCS_INI_LOG_WINDOW_SHOW_ERROR, std::to_string(showError));
}

void LoggingWindow::clear() {
    buf.clear();
    lineOffsets.clear();
    lineOffsets.push_back(0);
    updateVisibleLines();
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
    updateVisibleLines();
}

void LoggingWindow::updateVisibleLines() {
    visibleLines.clear();

    const char* bufBegin = buf.begin();
    const char* bufEnd = buf.end();

    for (int lineNo = 0; lineNo < lineOffsets.size() - 1; lineNo++) {
        const char* lineStart = bufBegin + lineOffsets[lineNo];
        const char* lineEnd = bufBegin + lineOffsets[lineNo + 1];
        bool show = true;

        // clang-format off
        switch (logLevels[lineNo]) {
            case spdlog::level::debug: show = showDebug; break;
            case spdlog::level::info:  show = showInfo;  break;
            case spdlog::level::warn:  show = showWarn;  break;
            case spdlog::level::err:   show = showError; break;
        }
        // clang-format on

        if (!show || (filter.IsActive() && !filter.PassFilter(lineStart, lineEnd))) {
            continue;
        }

        visibleLines.push_back(lineNo);
    }
}
