#ifndef LED_H
#define LED_H

#include <imgui.h>

extern const ImU32 LED_RED;
extern const ImU32 LED_ORANGE;
extern const ImU32 LED_YELLOW;
extern const ImU32 LED_GREEN;
extern const ImU32 LED_BLUE;
extern const ImU32 LED_PURPLE;

ImTextureID LoadLedTexture(const char* filePath);
void Led(bool open, const ImVec2& size = ImVec2(200, 200), ImU32 color = IM_COL32(255, 0, 0, 255));

#endif // LED_H
