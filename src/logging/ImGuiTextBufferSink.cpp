#include "ImGuiTextBufferSink.h"

#include "LoggingWindow.h"

/**
 * @brief Add log to an ImGui text buffer
 */
void ImGuiTextBufferSink::sink_it_(const spdlog::details::log_msg& msg) {
    spdlog::memory_buf_t formatted;
    formatter_->format(msg, formatted);
    LoggingWindow::addLog(formatted.data(), formatted.data() + formatted.size());
}

/**
 * @brief Implementation of flush_ pure virtual function that does nothing
 */
void ImGuiTextBufferSink::flush_() {
    // ImGuiTextBuffer directly appends data to an internal buffer and does not need a flush function
};
