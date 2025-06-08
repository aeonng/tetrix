//utility.cpp
#include "Utility.hpp"

namespace TextUtil {
    static Font regularFont;
    static Font boldFont;

    void loadFonts() {
        regularFont = LoadFont("assets/fonts/IMFellEnglish-Regular.ttf");
        boldFont = LoadFont("assets/fonts/IMFellEnglish-Regular.ttf");
    }

    void unloadFonts() {
        UnloadFont(regularFont);
        UnloadFont(boldFont);
    }

    void drawText(const std::string& text, Vector2 position, float fontSize, Color color) {
        DrawTextEx(regularFont, text.c_str(), position, fontSize, 1, color);
    }

    void drawTextTitle(const std::string& text, Vector2 position, float fontSize, Color color) {
        DrawTextEx(boldFont, text.c_str(), position, fontSize, 2, color);
    }
}

namespace DrawUtil {
    constexpr int GHOST_ID_OFFSET = 100;
    TextureMap blockTextures;
    TextureMap itemTextures;
    TextureMap backgroundTextures;

    void loadBlockTextures() {
        // 일반 block 텍스처 로드 및 ghost 버전 생성
        for (int i = 0; i <= 6; ++i) {
            std::string basePath;
            switch (i) {
                case 0: basePath = "block_red"; break;
                case 1: basePath = "block_orange"; break;
                case 2: basePath = "block_yellow"; break;
                case 3: basePath = "block_green"; break;
                case 4: basePath = "block_blue"; break;
                case 5: basePath = "block_purple"; break;
                case 6: basePath = "block_cyan"; break;
            }

            std::string path = "assets/images/" + basePath + ".png";
            Image img = LoadImage(path.c_str());
            ImageResize(&img, CELL_SIZE, CELL_SIZE);
            blockTextures[i] = LoadTextureFromImage(img);

            // ghost 텍스처 생성: 밝기 낮추고 투명도 추가
            Image ghostImg = ImageCopy(img);
            ImageColorTint(&ghostImg, Fade(WHITE, 0.3f));
            blockTextures[i + GHOST_ID_OFFSET] = LoadTextureFromImage(ghostImg);

            UnloadImage(img);
            UnloadImage(ghostImg);
        }
    }

    void unloadBlockTextures() {
        for (auto& [id, tex] : blockTextures)
            UnloadTexture(tex);
        blockTextures.clear();
    }

    void loadItemTextures() {
        for (int i = 0; i <= 3; ++i) {
            std::string basePath;
            switch (i) {
                case 0: basePath = "item_clearline_1"; break;
                case 1: basePath = "item_clearline_2"; break;
                case 2: basePath = "item_addline"; break;
                case 3: basePath = "item_opponent_addline"; break;
            }

            std::string path = "assets/images/" + basePath + ".png";
            Image img = LoadImage(path.c_str());
            ImageResize(&img, CELL_SIZE, CELL_SIZE);
            itemTextures[i] = LoadTextureFromImage(img);

            // ghost 텍스처 생성: 밝기 낮추고 투명도 추가
            Image ghostImg = ImageCopy(img);
            ImageColorTint(&ghostImg, Fade(WHITE, 0.3f));
            itemTextures[i + GHOST_ID_OFFSET] = LoadTextureFromImage(ghostImg);

            UnloadImage(img);
            UnloadImage(ghostImg);
        }
    }

    void unloadItemTextures() {
        for (auto& [id, tex] : itemTextures)
            UnloadTexture(tex);
        itemTextures.clear();
    }

    void loadBackgroundTextures() {
        // 타이틀 화면용 background ID = 0
        std::string path = "assets/info_pages/title.jpg";
        Image img = LoadImage(path.c_str());
        Texture2D tex = LoadTextureFromImage(img);
        backgroundTextures[0] = tex;
        UnloadImage(img);

        // 랭킹판 화면용 background ID = 1
        std::string boardPath = "assets/info_pages/board.png";
        Image boardImg = LoadImage(boardPath.c_str());
        Texture2D boardTex = LoadTextureFromImage(boardImg);
        backgroundTextures[1] = boardTex;
        UnloadImage(boardImg);

        // 게임 화면용 background ID = 2
        std::string scenePath = "assets/info_pages/gamescene.png";
        Image sceneImg = LoadImage(scenePath.c_str());
        Texture2D sceneTex = LoadTextureFromImage(sceneImg);
        backgroundTextures[2] = sceneTex;
        UnloadImage(sceneImg);

        // 게임 방법용 background ID = 3
        std::string howPath = "assets/info_pages/howtoplay.png";
        Image howImg = LoadImage(howPath.c_str());
        Texture2D howTex = LoadTextureFromImage(howImg);
        backgroundTextures[3] = howTex;
        UnloadImage(howImg);
    }

    void unloadBackgroundTextures() {
        for (auto& [id, tex] : backgroundTextures)
            UnloadTexture(tex);
        backgroundTextures.clear();
    }

    void drawBlock(int gridX, int gridY, float x_offset, float y_offset, int colorId) {
        float px = gridX * CELL_SIZE;
        float py = gridY * CELL_SIZE;

        if (colorId >= GHOST_ID_OFFSET) {
            int baseId = colorId - GHOST_ID_OFFSET;
            auto it2 = blockTextures.find(baseId);
            if (it2 != blockTextures.end()) {
                DrawTexture(it2->second, px + x_offset, py + y_offset, Fade(WHITE, 0.3f));
                return;
            }
        } else {
            auto it = blockTextures.find(colorId);
            if (it != blockTextures.end()) {
                DrawTexture(it->second, px + x_offset, py + y_offset, WHITE);
                return;
            }
        }

        if (colorId == -2) {
            DrawRectangle(px + x_offset, py + y_offset, CELL_SIZE, CELL_SIZE, GRAY);
        }
    }


    void drawItem(int gridX, int gridY, float x_offset, float y_offset, int colorId) {
        float px = gridX * CELL_SIZE;
        float py = gridY * CELL_SIZE;

        if (colorId >= GHOST_ID_OFFSET) {
            int baseId = colorId - GHOST_ID_OFFSET;
            auto it2 = itemTextures.find(baseId);
            if (it2 != itemTextures.end()) {
                DrawTexture(it2->second, px + x_offset, py + y_offset, Fade(WHITE, 0.3f));
                return;
            }
        } else {
            auto it = itemTextures.find(colorId);
            if (it != itemTextures.end()) {
                DrawTexture(it->second, px + x_offset, py + y_offset, WHITE);
                return;
            }
        }
    }

    void drawFallbackCell(int gridX, int gridY, float x_offset, float y_offset, Color fillColor) {
        int px = gridX * CELL_SIZE;
        int py = gridY * CELL_SIZE;

        DrawRectangle(px + x_offset, py + y_offset, CELL_SIZE, CELL_SIZE, fillColor);
        //DrawRectangleLines(px, py, CELL_SIZE, CELL_SIZE, BLACK);
    }
}

namespace AudioUtil {
    Sound moveSound;
    Music mainMusic;
    Sound deleteSound;
    Sound ItemSound;
    Sound blockSound;
    Sound comboSound;

    void loadSounds() {
        mainMusic = LoadMusicStream("assets/bgm/A_Bleeding_Fight.mp3");
        moveSound = LoadSound("assets/bgm/button.mp3");
        deleteSound = LoadSound("assets/bgm/get_item.mp3");
        ItemSound = LoadSound("assets/bgm/line_delete.mp3");
        blockSound = LoadSound("assets/bgm/button.mp3");
        comboSound = LoadSound("assets/bgm/combo.mp3");
        PlayMusicStream(mainMusic);
    }

    void unloadSounds() {
        UnloadSound(moveSound);
        UnloadSound(deleteSound);
        UnloadSound(ItemSound);
        UnloadSound(blockSound);
        UnloadSound(comboSound);
        UnloadMusicStream(mainMusic);
    }

    void playMoveSound() {
        PlaySound(moveSound);
    }

    void playDeleteSound() {
        PlaySound(deleteSound);
    }

    void playItemSound() {
        PlaySound(ItemSound);
    }

    void playblockSound() {
        PlaySound(blockSound);
    }

    void playcomboSound() {
        PlaySound(comboSound);
    }

    void playMainSound() {
        PlayMusicStream(mainMusic); 
    }

    void updateMainSound() {
        UpdateMusicStream(mainMusic);  
    }
}