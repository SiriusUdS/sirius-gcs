#include "Logging.h"

#include "ImGuiTextBufferSink.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Logging {
std::vector<spdlog::sink_ptr> appLoggerSinks;
std::vector<spdlog::sink_ptr> dataLoggerSinks;

void initAppLogger();
void initDataLogger();
} // namespace Logging

void Logging::init() {
    spdlog::flush_every(std::chrono::seconds(5));
    initAppLogger();
}

void Logging::initAppLogger() {
    spdlog::sink_ptr appConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    spdlog::sink_ptr appFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/app.log");
    std::shared_ptr<ImGuiTextBufferSink> appImguiSink = std::make_shared<ImGuiTextBufferSink>();

    appLoggerSinks = {appConsoleSink, appFileSink, appImguiSink};

    appLogger = std::make_shared<spdlog::logger>("app", appLoggerSinks.begin(), appLoggerSinks.end());
    appLogger->set_level(spdlog::level::trace);
    appLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
}

void Logging::initDataLogger() {
    spdlog::sink_ptr dataConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    spdlog::sink_ptr dataFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/data.log");

    dataLoggerSinks = {dataConsoleSink, dataFileSink};

    dataLogger = std::make_shared<spdlog::logger>("data", dataLoggerSinks.begin(), dataLoggerSinks.end());
    dataLogger->set_level(spdlog::level::info);
    dataLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
}
