#ifndef LOGGING_H
#define LOGGING_H

#include "LoggingWindow.h"

#include <spdlog/spdlog.h>

namespace Logging {
void initSpdLog();
void updateSpdLog();
void linkLoggingWindow(LoggingWindow* loggingWindow);
void removeLoggingWindow();

extern std::vector<spdlog::sink_ptr> _sinks;
} // namespace Logging

#define GCS_LOG_DEBUG(msg, ...) spdlog::debug(msg, __VA_ARGS__)
#define GCS_LOG_ERROR(msg, ...) spdlog::error(msg, __VA_ARGS__)
#define GCS_LOG_INFO(msg, ...) spdlog::info(msg, __VA_ARGS__)
#define GCS_LOG_TRACE(msg, ...) spdlog::trace(msg, __VA_ARGS__)
#define GCS_LOG_WARN(msg, ...) spdlog::warn(msg, __VA_ARGS__)

#endif // LOGGING_H
