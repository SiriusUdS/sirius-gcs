#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
// Window names
constexpr const char* GCS_LOGGING_WINDOW_ID = "Logs";
constexpr const char* GCS_MAP_WINDOW_ID = "Map";
constexpr const char* GCS_BASE_PLOT_WINDOW_ID = "Plot - ";
constexpr const char* GCS_CONTROLS_WINDOW_ID = "Controls";
constexpr const char* GCS_SWITCHES_WINDOW_ID = "Switches";

// Plot
constexpr size_t PLOT_MAX_DATA_SIZE_ORIGINAL = 100'000;
constexpr size_t PLOT_DATA_AMOUNT_TO_DROP_IF_MAX_REACHED = 10'000;
constexpr size_t PLOT_MAX_DATA_SIZE_COMPRESSED = 20'000;
constexpr size_t PLOT_TARGET_DATA_SIZE_COMPRESSED = 10'000;
constexpr const char* PLOT_AXIS_TIMESTAMP_NAME = "Timestamp (ms)";

// Dock space names
constexpr const char* GCS_LOGGING_DOCKSPACE = "LogSpace";
constexpr const char* GCS_MAP_DOCKSPACE = "MapSpace";
constexpr const char* GCS_PLOT_DOCKSPACE = "MainDockSpace";

// .ini file & sections (where application state is saved)
constexpr const char* GCS_INI_SECTION = "GCS";
constexpr const char* GCS_INI_FILENAME = "sirius_gcs.ini";
constexpr const char* GCS_INI_MAP_WINDOW_MAP_VIEW = "map_window_map_view";
constexpr const char* GCS_INI_BASE_PLOT_WINDOW_AUTO_FIT = "_plot_window_auto_fit";
constexpr const char* GCS_INI_BASE_PLOT_WINDOW_SHOW_COMPRESSED_DATA = "_plot_window_show_compressed_data";
constexpr const char* GCS_INI_LOG_WINDOW_AUTO_SCROLL = "log_window_auto_scroll";
constexpr const char* GCS_INI_LOG_WINDOW_SHOW_DEBUG = "log_window_show_debug";
constexpr const char* GCS_INI_LOG_WINDOW_SHOW_INFO = "log_window_show_info";
constexpr const char* GCS_INI_LOG_WINDOW_SHOW_WARN = "log_window_show_warn";
constexpr const char* GCS_INI_LOG_WINDOW_SHOW_ERROR = "log_window_show_error";

// HelloImGui specific params
constexpr const char* HELLO_IMGUI_INI_FILENAME = "hello_imgui.ini";
constexpr const char* HELLO_IMGUI_WINDOW_TITLE = "Sirius GCS";
constexpr const char* HELLO_IMGUI_MENU_APP_TITLE = "Sirius GCS";

// Serial COM
constexpr size_t RECV_BUF_SIZE = 10'000;
constexpr size_t RECV_PACKET_MAX_HEADER_SIZE = 4;
} // namespace Constants

#endif // CONSTANTS_H
