//modeselectpage.cpp
#include "ModeSelectPage.hpp"
#include "Utility.hpp"
#include <raylib.h>

ModeSelectPage::ModeSelectPage(bool single)
    : singleMode(single), columnIndex(0), modeIndex(0), editingGoal(false), difficultyIndex(0), \
    sprintIndex(0), isReady(false), editingNickname(false) {}

GameState ModeSelectPage::update() {
    if (editingNickname) {
        int key = GetCharPressed();
        while (key > 0) {
            if (nickname.length() < 10 && key >= 32 && key <= 126)
                nickname += static_cast<char>(key);
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !nickname.empty())
            nickname.pop_back();
        else if (IsKeyPressed(KEY_ENTER)) {
            AudioUtil::playMoveSound(); 
            editingNickname = !editingNickname;
        }
    } 
    else if (editingGoal && columnIndex == 0 && modeIndex == 1) {
        if (IsKeyPressed(KEY_DOWN)) {
            AudioUtil::playMoveSound(); 
            sprintIndex = (sprintIndex + 1) % 4;
        }
        else if (IsKeyPressed(KEY_UP)) {
            AudioUtil::playMoveSound(); 
            sprintIndex = (sprintIndex - 1 + 4) % 4;
        }
        else if (IsKeyPressed(KEY_ENTER)) {
            AudioUtil::playMoveSound(); 
            editingGoal = !editingGoal;
        }
    }
    else {
        if (IsKeyPressed(KEY_LEFT)) {
            AudioUtil::playMoveSound(); 
            columnIndex = (columnIndex - 1 + (singleMode ? 3 : 2)) % (singleMode ? 3 : 2);
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            AudioUtil::playMoveSound(); 
            columnIndex = (columnIndex + 1) % (singleMode ? 3 : 2);
        }

        if (IsKeyPressed(KEY_UP)) {
            AudioUtil::playMoveSound(); 
            if (columnIndex == 0) modeIndex = (modeIndex - 1 + 2) % 2;
            else if (columnIndex == 1) difficultyIndex = (difficultyIndex - 1 + 3) % 3;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            AudioUtil::playMoveSound(); 
            if (columnIndex == 0) modeIndex = (modeIndex + 1) % 2;
            else if (columnIndex == 1) difficultyIndex = (difficultyIndex + 1) % 3;
        }

        if (singleMode && columnIndex == 2) {
            if (IsKeyPressed(KEY_ENTER)) {
                AudioUtil::playMoveSound(); 
                editingNickname = !editingNickname;
            }
        }

        if (columnIndex == 0 && modeIndex == 1) {  
            if (IsKeyPressed(KEY_ENTER)) {
                AudioUtil::playMoveSound(); 
                editingGoal = !editingGoal;
            }
        }

        if (IsKeyPressed(KEY_H)) {
            AudioUtil::playMoveSound(); 
            return GameState::START;
        }
    }

    if (IsKeyPressed(KEY_S) && !editingNickname && !editingGoal && (!singleMode || (singleMode && !nickname.empty()))) {
            AudioUtil::playMoveSound(); 
            isReady = true;
            return GameState::GAME_RUNNING;
    }

    return GameState::MODE_SELECT;
}

void ModeSelectPage::draw() {
    auto it = DrawUtil::backgroundTextures.find(1); 
    if (it != DrawUtil::backgroundTextures.end()) {
        DrawTexture(it->second, 0, 0, WHITE);
    } else {
        ClearBackground(RAYWHITE);  
    }

    std::string title = singleMode ? "SINGLE PLAYER SETUP" : "DUAL PLAYER SETUP";
    float titleWidth = MeasureText(title.c_str(), 40);
    TextUtil::drawTextTitle(title, {30+(1280 - titleWidth) / 2.0f, 200}, 45, GOLD);
    
    TextUtil::drawText("     PRESS H TO RETURN  ", {820, 550}, 21, LIGHTGRAY);
    TextUtil::drawText("     PRESS S TO START   ", {820, 575}, 21, RED);

    // Mode
    Color col0_color = (columnIndex == 0) ? GOLD : WHITE;
    TextUtil::drawText("GAME MODE:", {250, 270}, 30, col0_color);
    for (int i = 0; i < modes.size(); ++i) {
        Color color = (i == modeIndex) ? GOLD : WHITE;
        TextUtil::drawText(modes[i], {260, 320.0f + i * 50}, 28, color);
    }
    if (modeIndex == 1) {
        if (editingGoal) {
            for (int i = 0; i < sprint.size(); i++) {
                Color color = (i == sprintIndex) ? GOLD : GRAY;
                TextUtil::drawText(std::to_string(sprint[i])+" ROWS", {260, 400.0f + i * 30}, 28, color);
            }
        }
        else {
            TextUtil::drawText(std::to_string(sprint[sprintIndex])+" ROWS", {260, 400.0f}, 28, GRAY);
        }
    }

    // Difficulty
    Color col1_color = (columnIndex == 1) ? GOLD : WHITE;
    TextUtil::drawText("DIFFICULTY:", {550, 270}, 30, col1_color);
    for (int i = 0; i < difficulties.size(); ++i) {
        Color color = (i == difficultyIndex) ? GOLD : WHITE;
        TextUtil::drawText(difficulties[i], {570, 320.0f + i * 50}, 28, color);
    }

    // Nickname (only in single mode)
    if (singleMode) {
        Color col2_color = (columnIndex == 2) ? GOLD : WHITE;
        TextUtil::drawText("NICKNAME:", {900, 270}, 30, col2_color);
        Color color = (columnIndex == 2) ? GOLD : WHITE;
        std::string displayName = editingNickname ? nickname + "_" : nickname;
        TextUtil::drawText(displayName, {920, 320}, 28, color);
    }
}

std::string ModeSelectPage::getNickname() const { 
    if (singleMode) return nickname; 
    return "\0";
}
int ModeSelectPage::getModeIndex() const { return modeIndex; }
int ModeSelectPage::getDifficultyIndex() const { return difficultyIndex; }
int ModeSelectPage::getSprintGoal() const {
    if (modeIndex != 1) {
        return -1;
    }
    else {
        return sprint[sprintIndex];
    }
}