#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
// Window names
constexpr const char* GCS_LED_WINDOW_ID = "LED";
constexpr const char* GCS_LOGGING_WINDOW_ID = "Logs";
constexpr const char* GCS_MAP_WINDOW_ID = "Map";
constexpr const char* GCS_PLOT_WINDOW_ID = "Plot";
constexpr const char* GCS_CONTROLS_WINDOW_ID = "Controls";

// Map component
constexpr const size_t GCS_MAP_MAX_TILES_DOWNLOAD = 100'000;
constexpr const size_t GCS_TILE_REQUEST_LIMIT = 25;

// Dock space names
constexpr const char* GCS_LED_DOCKSPACE = "MainDockSpace";
constexpr const char* GCS_LOGGING_DOCKSPACE = "LogSpace";
constexpr const char* GCS_MAP_DOCKSPACE = "MapSpace";
constexpr const char* GCS_PLOT_DOCKSPACE = "MainDockSpace";

// .ini file & sections (where application state is saved)
constexpr const char* GCS_INI_SECTION = "GCS";
constexpr const char* GCS_INI_FILENAME = "sirius_gcs.ini";
constexpr const char* GCS_INI_MAP_WINDOW_MAP_VIEW = "map_window_map_view";
constexpr const char* GCS_INI_PLOT_WINDOW_AUTO_FIT = "plot_window_auto_fit";
constexpr const char* GCS_INI_LOG_WINDOW_AUTO_SCROLL = "log_window_auto_scroll";

// HelloImGui specific params
constexpr const char* HELLO_IMGUI_INI_FILENAME = "hello_imgui.ini";
constexpr const char* HELLO_IMGUI_WINDOW_TITLE = "Sirius GCS";
constexpr const char* HELLO_IMGUI_MENU_APP_TITLE = "Sirius GCS";

// Font related params
constexpr const char* IMGUI_MAIN_FONT_FILENAME = "assets/fonts/Nunito-Regular.ttf";
constexpr float IMGUI_MAIN_FONT_SIZE = 28.0f;

// Serial COM
constexpr size_t RECV_BUF_SIZE = 10000;
constexpr size_t WIN_REG_VAL_BUF_SIZE = 256;
} // namespace Constants

#endif // CONSTANTS_H
