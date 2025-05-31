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

// Map component
constexpr const size_t GCS_MAP_MAX_TILES_DOWNLOAD = 100'000;
constexpr const size_t GCS_TILE_REQUEST_LIMIT = 25;

// Dock space names
constexpr const char* GCS_LOGGING_DOCKSPACE = "LogSpace";
constexpr const char* GCS_MAP_DOCKSPACE = "MapSpace";
constexpr const char* GCS_PLOT_DOCKSPACE = "MainDockSpace";

// .ini file & sections (where application state is saved)
constexpr const char* GCS_INI_SECTION = "GCS";
constexpr const char* GCS_INI_FILENAME = "sirius_gcs.ini";
constexpr const char* GCS_INI_MAP_WINDOW_MAP_VIEW = "map_window_map_view";
constexpr const char* GCS_INI_BASE_PLOT_WINDOW_AUTO_FIT = "_plot_window_auto_fit";
constexpr const char* GCS_INI_LOG_WINDOW_AUTO_SCROLL = "log_window_auto_scroll";

// HelloImGui specific params
constexpr const char* HELLO_IMGUI_INI_FILENAME = "hello_imgui.ini";
constexpr const char* HELLO_IMGUI_WINDOW_TITLE = "Sirius GCS";
constexpr const char* HELLO_IMGUI_MENU_APP_TITLE = "Sirius GCS";

// Font related params
constexpr const char* IMGUI_MAIN_FONT_FILENAME = "assets/fonts/Nunito-Regular.ttf";
constexpr float IMGUI_MAIN_FONT_SIZE = 28;

// Temperature sensor
constexpr float ADC_MIN_TEMPERATURE = 10;
constexpr float ADC_MAX_TEMPERATURE = 4090;
constexpr int MAX_TEMPERATURE = 100;
constexpr int MIN_TEMPERATURE = 0;
constexpr float CONTROL_RESISTANCE = 10'000.0f;
constexpr float ADDITIVE_FACTOR = 4096;
typedef struct {
    float TEMPERATURE;
    float RESISTANCE;
} RT_Point;
constexpr RT_Point RT_TABLE[] = {
  {-30, 1733200}, {-20, 959000}, {-10, 551410}, {0, 327240}, {10, 199990}, {20, 125250}, {25, 100000}, {30, 81000},
  {40, 53500},    {50, 35900},   {60, 25000},   {70, 17550}, {80, 12540},  {90, 9100},   {100, 6710}}; // Resistor-Temperature values table
constexpr int RT_TABLE_SIZE = sizeof(Constants::RT_TABLE) / sizeof(Constants::RT_TABLE[0]);

// Load cell
constexpr float ADC_MIN_LOADCELL = 0;
constexpr float ADC_MAX_LOADCELL = 4095;
constexpr float LOADCELL_SCALE_FACTOR = 0.01f;

// Serial COM
constexpr size_t SERIAL_TASK_LOOPS_PER_SECOND = 20;
constexpr size_t SERIAL_CONNECTION_ATTEMPT_DELAY_IN_SECS = 3;
constexpr size_t SERIAL_MAX_CONSECUTIVE_FAILED_READS_BEFORE_FAILURE = 100;
constexpr size_t SERIAL_MAX_CONSECUTIVE_FAILED_WRITES_BEFORE_FAILURE = 100;
constexpr size_t RECV_BUF_SIZE = 25'000;
constexpr size_t RECV_BYTES_TO_READ_PER_SECOND = 19'200;
constexpr size_t RECV_PACKET_MAX_SIZE = 64;
constexpr size_t WIN_REG_VAL_BUF_SIZE = 256;

// Commands
constexpr size_t COMMAND_MAX_SIZE = 256;
constexpr size_t COMMAND_STORAGE_MAX_SIZE = 10;
constexpr size_t COMMAND_TIME_BEFORE_RESENDING_MS = 100;
} // namespace Constants

#endif // CONSTANTS_H
