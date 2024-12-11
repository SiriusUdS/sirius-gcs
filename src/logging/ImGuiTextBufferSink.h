#ifndef IMGUITEXTBUFFERSINK_H
#define IMGUITEXTBUFFERSINK_H

#include "LoggingWindow.h"

#include <spdlog/sinks/base_sink.h>

class ImGuiTextBufferSink : public spdlog::sinks::base_sink<std::mutex> {
public:
    void linkLoggingWindow(LoggingWindow* window);
    void unlinkLoggingWindow();

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override;
    void flush_() override;

private:
    LoggingWindow* loggingWindow;
};

#endif // IMGUITEXTBUFFERSINK_H
