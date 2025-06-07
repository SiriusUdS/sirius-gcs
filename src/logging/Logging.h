#ifndef LOGGING_H
#define LOGGING_H

#include <spdlog/spdlog.h>

class ImGuiTextBufferSink;

namespace Logging {
void init();

extern spdlog::sink_ptr consoleSink;
extern spdlog::sink_ptr fileSink;
extern std::shared_ptr<ImGuiTextBufferSink> imguiSink;
extern std::vector<spdlog::sink_ptr> sinks;
} // namespace Logging

#define GCS_LOG_DEBUG(msg, ...) spdlog::debug(msg, __VA_ARGS__)
#define GCS_LOG_ERROR(msg, ...) spdlog::error(msg, __VA_ARGS__)
#define GCS_LOG_INFO(msg, ...) spdlog::info(msg, __VA_ARGS__)
#define GCS_LOG_WARN(msg, ...) spdlog::warn(msg, __VA_ARGS__)

#endif // LOGGING_H
