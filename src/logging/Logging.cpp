#include "Logging.h"

#include "ImGuiTextBufferSink.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Logging {
std::vector<spdlog::sink_ptr> _sinks;
}

void Logging::initSpdLog() {
    const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/gcs-log.txt");
    _sinks = {console_sink, file_sink};
    updateSpdLog();
}

void Logging::updateSpdLog() {
    const auto logger = std::make_shared<spdlog::logger>("multi_sink", _sinks.begin(), _sinks.end());
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_every(std::chrono::seconds(5));
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
}

void Logging::linkLoggingWindow(LoggingWindow* loggingWindow) {
    const auto imgui_sink = std::make_shared<ImGuiTextBufferSink>(loggingWindow);
    _sinks.push_back(imgui_sink);
    updateSpdLog();
}

void Logging::removeLoggingWindow() {
    _sinks.pop_back(); // TODO - Store position of each type of sink, add option to remove whichever type of sink we want
    updateSpdLog();
}
