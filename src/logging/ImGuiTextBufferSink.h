#ifndef IMGUITEXTBUFFERSINK_H
#define IMGUITEXTBUFFERSINK_H

#include "LoggingWindow.h"

#include <spdlog/sinks/base_sink.h>

/**
 * @class ImGuiTextBufferSink
 * @brief Sink that forwards logs to an ImGui textbuffer
 */
class ImGuiTextBufferSink : public spdlog::sinks::base_sink<std::mutex> {
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override;
    void flush_() override;
};

#endif // IMGUITEXTBUFFERSINK_H
