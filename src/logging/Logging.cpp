#include "Logging.h"

#include "ImGuiTextBufferSink.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Logging {
spdlog::sink_ptr consoleSink;
spdlog::sink_ptr fileSink;
std::shared_ptr<ImGuiTextBufferSink> imguiSink;
std::vector<spdlog::sink_ptr> sinks;
} // namespace Logging

void Logging::init() {
    consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/gcs-log.txt");
    imguiSink = std::make_shared<ImGuiTextBufferSink>();
    sinks = {consoleSink, fileSink, imguiSink};

    const auto logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_every(std::chrono::seconds(5));
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
}
