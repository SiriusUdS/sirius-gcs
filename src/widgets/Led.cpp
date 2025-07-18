#include "Led.h"

#include "Logging.h"

// clang-format off
 #include <glad/glad.h>
 #include <GLFW/glfw3.h>
// clang-format on

#include <stb_image.h>

const ImU32 LED_RED = IM_COL32(255, 0, 0, 255);
const ImU32 LED_ORANGE = IM_COL32(255, 165, 0, 255);
const ImU32 LED_YELLOW = IM_COL32(255, 255, 0, 255);
const ImU32 LED_GREEN = IM_COL32(0, 255, 0, 255);
const ImU32 LED_BLUE = IM_COL32(0, 0, 255, 255);
const ImU32 LED_PURPLE = IM_COL32(160, 32, 240, 255);

/**
 * @brief Loads the shading texture for the LED component
 * @param filePath File path of the LED shading image file
 * @return ImTextureID
 */
ImTextureID LoadLedTexture(const char* filePath) {
    int height, width, channels;

    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 0);
    if (!data) {
        GCS_APP_LOG_ERROR("Failed to load LED texture: {}", filePath);
        return NULL;
    }

    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, (channels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return (ImTextureID) (intptr_t) texture;
}

/**
 * @brief Displays an LED
 * @param open Whether the LED is shown as turned on/off
 * @param size Size of the LED
 * @param color Color of the LED
 * @return void
 */
void Led(bool open, const ImVec2& size, ImU32 color) {
    static ImTextureID openTexture = LoadLedTexture("assets/images/open_led.png");
    static ImTextureID closedTexture = LoadLedTexture("assets/images/closed_led.png");

    ImTextureID texture = open ? openTexture : closedTexture;

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);
    float radius = size.x / 2.0f;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddCircleFilled(center, radius, color);

    ImGui::SetCursorScreenPos(pos);
    ImGui::Image(texture, size);
}
