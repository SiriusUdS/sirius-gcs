#include "SerialComWindow.h"

#include "ComPortSelector.h"
#include "FontConfig.h"
#include "IniConfig.h"
#include "PacketRateMonitor.h"
#include "SerialCom.h"
#include "SerialConfig.h"
#include "SerialStateMonitor.h"
#include "SerialTask.h"

#include <algorithm>
#include <imgui.h>

namespace SerialComWindow {
enum RecvBufferDisplayMode { TEXT = 0, HEXA = 1 };

constexpr const char* INI_RECV_BUFFER_DISPLAY_MODE = "recv_buffer_display_mode";

void renderPacketRateTableRow(const char* packetName, double rate);
void recvBufferContentModal();
void updateRecvBufferContentDisplay(bool syncToCurrentBuffer);

int recvBufferDisplayMode{TEXT};
bool recvBufferDisplayPause{};
std::vector<char> recvBufferContentDisplay(SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE);
} // namespace SerialComWindow

void SerialComWindow::render() {
    const char* comStateText = "Unknown";
    if (!SerialTask::com.comOpened()) {
        comStateText = "Disconnected";
    } else {
        switch (SerialTask::serialStateMonitor.getState()) {
        case SerialStateMonitor::State::STARTING:
            comStateText = "Starting";
            break;
        case SerialStateMonitor::State::RESETTING:
            comStateText = "Resetting";
            break;
        case SerialStateMonitor::State::WORKING:
            comStateText = "Working";
            break;
        case SerialStateMonitor::State::NOT_WORKING:
            comStateText = "Not working";
            break;
        }
    }

    ImGui::Text("COM State: ");
    ImGui::SameLine();
    ImGui::Text(comStateText);

    if (ImGui::CollapsingHeader("Packet rates")) {
        if (ImGui::BeginTable("PacketRatesTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Packet Type");
            ImGui::TableSetupColumn("Packets/s");
            ImGui::TableHeadersRow();

            renderPacketRateTableRow("Engine telemetry", SerialTask::engineTelemetryPacketRateMonitor.getRatePerSecond());
            renderPacketRateTableRow("Filling station telemetry", SerialTask::fillingStationTelemetryPacketRateMonitor.getRatePerSecond());
            renderPacketRateTableRow("GS control", SerialTask::gsControlPacketRateMonitor.getRatePerSecond());
            renderPacketRateTableRow("Engine status", SerialTask::engineStatusPacketRateMonitor.getRatePerSecond());
            renderPacketRateTableRow("Filling station status", SerialTask::fillingStationStatusPacketRateMonitor.getRatePerSecond());
            ImGui::TableNextRow(); // Separator row for total
            renderPacketRateTableRow("Total", SerialTask::packetRateMonitor.getRatePerSecond());

            ImGui::EndTable();
        }
    }

    if (ImGui::Button("View RECV buffer content")) {
        ImGui::OpenPopup("RECV Buffer Content");
    }

    recvBufferContentModal();
}

void SerialComWindow::loadState(const mINI::INIStructure& ini) {
    if (ini.has(IniConfig::GCS_SECTION)) {
        if (ini.get(IniConfig::GCS_SECTION).has(INI_RECV_BUFFER_DISPLAY_MODE)) {
            recvBufferDisplayMode = std::stoi(ini.get(IniConfig::GCS_SECTION).get(INI_RECV_BUFFER_DISPLAY_MODE));
        }
    }
}

void SerialComWindow::saveState(mINI::INIStructure& ini) {
    ini[IniConfig::GCS_SECTION].set(INI_RECV_BUFFER_DISPLAY_MODE, std::to_string(recvBufferDisplayMode));
}

void SerialComWindow::renderPacketRateTableRow(const char* packetName, double rate) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text(packetName);
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%.1f", rate);
}

void SerialComWindow::recvBufferContentModal() {
    updateRecvBufferContentDisplay(!recvBufferDisplayPause);

    const ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    const ImVec2 minBoxSize = {400, 200};
    const ImVec2 maxBoxSize = {2000, 1200};
    const ImVec2 boxSize = {std::clamp(screenSize.x * 0.8f, minBoxSize.x, maxBoxSize.x), std::clamp(screenSize.y * 0.6f, minBoxSize.y, maxBoxSize.y)};
    const ImVec2 boxPos = {(screenSize.x - boxSize.x) * 0.5f - 10.0f, (screenSize.y - boxSize.y) * 0.5f - 60.0f};

    ImGui::SetNextWindowPos(boxPos, ImGuiCond_Always);

    const ImGuiWindowFlags modalFlags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove;

    if (ImGui::BeginPopupModal("RECV Buffer Content", nullptr, modalFlags)) {
        ImGui::RadioButton("Text", &recvBufferDisplayMode, TEXT);
        ImGui::SameLine();
        ImGui::RadioButton("Hexa", &recvBufferDisplayMode, HEXA);
        ImGui::SameLine();
        ImGui::Checkbox("Pause", &recvBufferDisplayPause);

        const char* displayBuf = recvBufferContentDisplay.data();

        ImGui::BeginChild("RecvBufferBox", boxSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushTextWrapPos(0.0f);
        ImGui::PushFont(FontConfig::codeFont);

        if (recvBufferDisplayMode == TEXT) {
            ImGui::TextUnformatted(displayBuf, displayBuf + SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE);
        } else {
            ImGui::BeginGroup();
            for (size_t i = 0; i < SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE; ++i) {
                unsigned char byte = static_cast<unsigned char>(displayBuf[i]);

                bool dim = byte == '\0';
                if (dim) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
                }

                char hexByte[4]; // 2 hex digits + space + null terminator
                std::snprintf(hexByte, sizeof(hexByte), "%02X ", byte);
                ImGui::TextUnformatted(hexByte);

                if (dim) {
                    ImGui::PopStyleColor();
                }

                if ((i + 1) % 16 != 0) {
                    ImGui::SameLine();
                }
            }
            ImGui::EndGroup();
        }

        ImGui::PopFont();
        ImGui::PopTextWrapPos();
        ImGui::EndChild();

        if (ImGui::Button("Close", {boxSize.x, 0})) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void SerialComWindow::updateRecvBufferContentDisplay(bool syncToCurrentBuffer) {
    const char* buf = (char*) SerialTask::com.getBuffer();
    const char zeroChar = recvBufferDisplayMode == TEXT ? '~' : '\0';

    if (syncToCurrentBuffer) {
        // Sync recv buffer display
        for (size_t i = 0; i < SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE; i++) {
            if (buf[i] == '\0') {
                recvBufferContentDisplay[i] = zeroChar;
            } else {
                recvBufferContentDisplay[i] = buf[i];
            }
        }
    } else {
        // Only modify how zeros are represented
        const char charToReplace = recvBufferDisplayMode == TEXT ? '\0' : '~';
        for (size_t i = 0; i < SerialConfig::PACKET_CIRCULAR_BUFFER_SIZE; i++) {
            if (recvBufferContentDisplay[i] == charToReplace) {
                recvBufferContentDisplay[i] = zeroChar;
            }
        }
    }
}
