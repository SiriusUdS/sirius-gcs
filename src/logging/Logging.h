#ifndef LOGGING_H
#define LOGGING_H

#include "ImGuiTextBufferSink.h"
#include "LoggingWindow.h"

#include <spdlog/spdlog.h>

namespace Logging {
void initSpdLog();
void linkLoggingWindow(LoggingWindow* loggingWindow);
void unlinkLoggingWindow();

extern spdlog::sink_ptr consoleSink;
extern spdlog::sink_ptr fileSink;
extern std::shared_ptr<ImGuiTextBufferSink> imguiSink;
extern std::vector<spdlog::sink_ptr> sinks;
} // namespace Logging

#define GCS_LOG_DEBUG(msg, ...) spdlog::debug(msg, __VA_ARGS__)
#define GCS_LOG_ERROR(msg, ...) spdlog::error(msg, __VA_ARGS__)
#define GCS_LOG_INFO(msg, ...) spdlog::info(msg, __VA_ARGS__)
#define GCS_LOG_TRACE(msg, ...) spdlog::trace(msg, __VA_ARGS__)
#define GCS_LOG_WARN(msg, ...) spdlog::warn(msg, __VA_ARGS__)

#endif // LOGGING_H
