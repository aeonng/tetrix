#include "StartPage.hpp"
#include "Utility.hpp"

StartPage::StartPage() {
    selectedIndex = 0;
}


GameState StartPage::update() {
    if (IsKeyPressed(KEY_UP)) {
        if (selectedIndex >= 2) {
            selectedIndex -= 2;
            AudioUtil::playMoveSound(); 
        }
    }
    if (IsKeyPressed(KEY_DOWN)) {
        if (selectedIndex <= 1) {
            selectedIndex += 2;
            AudioUtil::playMoveSound();  
        }
    }
    if (IsKeyPressed(KEY_LEFT)) {
        if (selectedIndex % 2 != 0) {
            selectedIndex -= 1;
            AudioUtil::playMoveSound();
        }
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        if (selectedIndex % 2 == 0) {
            selectedIndex += 1;
            AudioUtil::playMoveSound();
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        switch (selectedIndex) {
            case 0: return GameState::HOW_TO_PLAY;
            case 1: return GameState::RANKING;
            case 2:
                isSinglePlayer = false;
                return GameState::MODE_SELECT;
            case 3:
                isSinglePlayer = true;
                return GameState::MODE_SELECT;
        }
    }

    return GameState::START;
}

void StartPage::draw() {
    // 배경 그리기
    auto it = DrawUtil::backgroundTextures.find(0); // 타이틀 배경 ID = 0
    if (it != DrawUtil::backgroundTextures.end()) {
        DrawTexture(it->second, 0, 0, WHITE);
    } else {
        ClearBackground(RAYWHITE);  
    }

    // 버튼 좌표 설정 (2×2 배열)
    float startX = 400.0f;  // 첫 열 위치 (조정 가능)
    float startY = 430.0f;  // 첫 행 위치 (조정 가능)
    float spacingX = 320.0f;  // 열 간격 (버튼 간격)
    float spacingY = 75.0f;  // 행 간격

    for (int i = 0; i < 4; ++i) {
        int row = i / 2;
        int col = i % 2;

        float posX = startX + col * spacingX;
        float posY = startY + row * spacingY;

        Color color = (i == selectedIndex) ? RED : WHITE;
        TextUtil::drawText(menuItems[i], {posX, posY}, 30, color);
    }

}  