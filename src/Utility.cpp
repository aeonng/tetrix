//utility.cpp
#include "Utility.hpp"

namespace TextUtil {
    Font regularFont;
    Font boldFont;
    //Font numFont;

    void loadFonts() {
        regularFont = LoadFont("assets/fonts/IMFellEnglish-Regular.ttf");
        boldFont = LoadFont("assets/fonts/IMFellEnglish-Regular.ttf");
        //numFont = LoadFont("assets/fonts/chinese.msyh.ttf");
    }

    void unloadFonts() {
        UnloadFont(regularFont);
        UnloadFont(boldFont);
        //UnloadFont(numFont);
    }

    void drawText(const std::string& text, Vector2 position, float fontSize, Color color) {
        int numberFontSize = fontSize * 1.6f; // 숫자는 더 크게 표시
        int spacing = 1;
        Vector2 pos = position;
        int yOffset = fontSize * 0.4f;

        size_t i = 0;
        while (i < text.size()) {
            bool isDigit = std::isdigit(text[i]);

            size_t start = i;
            while (i < text.size() && std::isdigit(text[i]) == isDigit) {
                i++;
            }

            std::string segment = text.substr(start, i - start);
            int currentFontSize = isDigit ? numberFontSize : fontSize;
            Vector2 drawPos = pos;
            if (isDigit) {
                drawPos.y -= yOffset; // 숫자는 위로 살짝 올림
            }

            DrawTextEx(regularFont, segment.c_str(), drawPos, currentFontSize, spacing, color);

            Vector2 textSize = MeasureTextEx(regularFont, segment.c_str(), currentFontSize, spacing);
            pos.x += textSize.x;
        }
    } 

    void drawTextTitle(const std::string& text, Vector2 position, float fontSize, Color color) {
        int numberFontSize = fontSize * 1.6f; // 숫자 크기 더 크게 설정
        int spacing = 2;
        Vector2 pos = position;
        int yOffset = fontSize * 0.4f;

        size_t i = 0;
        while (i < text.size()) {
            // 현재 문자가 숫자인지 확인
            bool isDigit = std::isdigit(text[i]);

            // 같은 타입(숫자 or 문자)인 문자열 블록 추출
            size_t start = i;
            while (i < text.size() && std::isdigit(text[i]) == isDigit) {
                i++;
            }

            std::string segment = text.substr(start, i - start);
            int currentFontSize = isDigit ? numberFontSize : fontSize;
            Vector2 drawPos = pos;
            if (isDigit) {
                drawPos.y -= yOffset; // 숫자는 위로 살짝 올림
            }
            // 현재 블록 그리기
            DrawTextEx(boldFont, segment.c_str(), pos, currentFontSize, spacing, color);

            // 위치 업데이트
            Vector2 textSize = MeasureTextEx(boldFont, segment.c_str(), currentFontSize, spacing);
            pos.x += textSize.x;
        }
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

        // 게임 방법용1 background ID = 3
        std::string howPath1 = "assets/info_pages/howtoplay1.png";
        Image howImg1 = LoadImage(howPath1.c_str());
        Texture2D howTex1 = LoadTextureFromImage(howImg1);
        backgroundTextures[3] = howTex1;
        UnloadImage(howImg1);

        // 게임 방법용2 background ID = 4
        std::string howPath2 = "assets/info_pages/howtoplay2.png";
        Image howImg2 = LoadImage(howPath2.c_str());
        Texture2D howTex2 = LoadTextureFromImage(howImg2);
        backgroundTextures[4] = howTex2;
        UnloadImage(howImg2);

        // 모드 선택용1 background ID = 5
        std::string mode1 = "assets/info_pages/MODESELECT1.png";
        Image modeImg1 = LoadImage(mode1.c_str());
        Texture2D modeTex1 = LoadTextureFromImage(modeImg1);
        backgroundTextures[5] = modeTex1;
        UnloadImage(modeImg1);

        // 모드 선택용2 background ID = 6
        std::string mode2 = "assets/info_pages/MODESELECT2.png";
        Image modeImg2 = LoadImage(mode2.c_str());
        Texture2D modeTex2 = LoadTextureFromImage(modeImg2);
        backgroundTextures[6] = modeTex2;
        UnloadImage(modeImg2);

        // 엔딩: 2P MODE (1P VIC) background ID = 7
        std::string END1 = "assets/info_pages/END1.png";
        Image ENDImg1 = LoadImage(END1.c_str());
        Texture2D ENDTex1 = LoadTextureFromImage(ENDImg1);
        backgroundTextures[7] = ENDTex1;
        UnloadImage(ENDImg1);

        // 엔딩: 2P MODE (2P VIC) background ID = 8
        std::string END2 = "assets/info_pages/END2.png";
        Image ENDImg2 = LoadImage(END2.c_str());
        Texture2D ENDTex2 = LoadTextureFromImage(ENDImg2);
        backgroundTextures[8] = ENDTex2;
        UnloadImage(ENDImg2);

        // 엔딩: 1P MODE marathon background ID = 9
        std::string END3 = "assets/info_pages/END3.png";
        Image ENDImg3 = LoadImage(END3.c_str());
        Texture2D ENDTex3 = LoadTextureFromImage(ENDImg3);
        backgroundTextures[9] = ENDTex3;
        UnloadImage(ENDImg3);

        // 엔딩: 1P MODE sprint success background ID = 10
        std::string END4 = "assets/info_pages/END4.png";
        Image ENDImg4 = LoadImage(END4.c_str());
        Texture2D ENDTex4 = LoadTextureFromImage(ENDImg4);
        backgroundTextures[10] = ENDTex4;
        UnloadImage(ENDImg4);

        // 엔딩: 1P MODE sprint failed background ID = 11
        std::string END5 = "assets/info_pages/END5.png";
        Image ENDImg5 = LoadImage(END5.c_str());
        Texture2D ENDTex5 = LoadTextureFromImage(ENDImg5);
        backgroundTextures[11] = ENDTex5;
        UnloadImage(ENDImg5);

        // 랭킹: marathon mode background ID = 12
        std::string RANK1 = "assets/info_pages/rank1.png";
        Image RANKImg1 = LoadImage(RANK1.c_str());
        Texture2D RANKTex1 = LoadTextureFromImage(RANKImg1);
        backgroundTextures[12] = RANKTex1;
        UnloadImage(RANKImg1);

        // 랭킹: sprint mode background ID = 13
        std::string RANK2 = "assets/info_pages/rank2.png";
        Image RANKImg2 = LoadImage(RANK2.c_str());
        Texture2D RANKTex2 = LoadTextureFromImage(RANKImg2);
        backgroundTextures[13] = RANKTex2;
        UnloadImage(RANKImg2);

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