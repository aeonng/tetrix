//utility.hpp
#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

namespace TextUtil {
    void loadFonts();
    void unloadFonts();

    void drawText(const std::string& text, Vector2 position, float fontSize, Color color);
    void drawTextTitle(const std::string& text, Vector2 position, float fontSize, Color color);
}

namespace DrawUtil {
    constexpr int CELL_SIZE = 30;

    // 텍스처 맵 타입
    using TextureMap = std::unordered_map<int, Texture2D>;

    // 블록 텍스처 로드
    void loadBlockTextures();
    void unloadBlockTextures();

    // 아이템 텍스처 로드
    void loadItemTextures();
    void unloadItemTextures();

    // 배경 텍스처 로드
    void loadBackgroundTextures();
    void unloadBackgroundTextures();

    // 색깔 ID → 텍스처 맵
    extern TextureMap blockTextures;
    extern TextureMap itemTextures;
    extern TextureMap backgroundTextures;

    // 블록 그리기: 색깔 ID 기준
    void drawBlock(int gridX, int gridY, float x_offset, float y_offset, int colorId);

    // 아이템 그리기: 색깔 ID 기준
    void drawItem(int gridX, int gridY, float x_offset, float y_offset, int colorId);

    // (텍스처 없는 경우 대체 그리기)
    void drawFallbackCell(int gridX, int gridY, float x_offset, float y_offset, Color fillColor);
}

namespace AudioUtil {
    void loadSounds();
    void unloadSounds();

    void playMoveSound();
    void playDeleteSound();
    void playItemSound();
    void playblockSound();
    void playcomboSound();
    void playMainSound();

    void updateMainSound();

    //extern Sound moveSound;
}