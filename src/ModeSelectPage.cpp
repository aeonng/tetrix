//modeselectpage.cpp
#include "ModeSelectPage.hpp"
#include "Utility.hpp"
#include <raylib.h>
#include <string.h>

ModeSelectPage::ModeSelectPage(bool single)
    : singleMode(single), columnIndex(0), modeIndex(0), editingGoal(false), difficultyIndex(0), \
    sprintIndex(0), isReady(false), editingNickname(false) {}

GameState ModeSelectPage::update() {
    if (editingNickname) {
        int key = GetCharPressed();
        while (key > 0) {;
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
    int bgId = singleMode ? 5 : 6; 

    auto it = DrawUtil::backgroundTextures.find(bgId);
    if (it != DrawUtil::backgroundTextures.end()) {
        DrawTexture(it->second, 0, 0, WHITE);
    } else {
        ClearBackground(RAYWHITE);
    }

    // Mode
    Color col0_color = (columnIndex == 0) ? (Color){198, 128, 43, 255} : WHITE;
    Color col1_color = (columnIndex == 1) ? (Color){198, 128, 43, 255} : WHITE;
    float modeTitleX = (singleMode ? 248 : 348);
    float modeOptionX = modeTitleX + (singleMode ? 2 : 10);
    float diffTitleX = singleMode ? 558 : 740;
    float diffOptionX = diffTitleX + (singleMode ? 2 : 10);

    // 적용
    TextUtil::drawText("GAME MODE:", {modeTitleX, 280}, 30, col0_color);
    /*for (int i = 0; i < modes.size(); ++i) {
        Color color = (i == modeIndex) ? (Color){198, 128, 43, 255} : GRAY;
        TextUtil::drawText(modes[i], {modeOptionX, 340.0f + i * 50}, 28, color);
    }*/
    for (int i = 0; i < modes.size(); ++i) {
        Color color = (i == modeIndex) ? (Color){198, 128, 43, 255} : GRAY;
        float textWidth = MeasureText(modes[i], 28);
        float centeredX = 78.0f + modeOptionX - textWidth / 2.0f;
        TextUtil::drawText(modes[i], {centeredX, 340.0f + i * 50}, 28, color);
    }
    if (modeIndex == 1) {
    if (editingGoal) {
        for (int i = 0; i < sprint.size(); i++) {
            Color color = (i == sprintIndex) ? (Color){198, 128, 43, 255} : GRAY;
            std::string text = std::to_string(sprint[i]) + " ROWS";
            float textWidth = MeasureText(text.c_str(), 30);
            float centeredX = 78.0f + modeOptionX - textWidth / 2.0f;
            TextUtil::drawText(text, {centeredX, 430.0f + i * 30}, 28, color);
        }
    }
    else {
        std::string text = std::to_string(sprint[sprintIndex]) + " ROWS";
        float textWidth = MeasureText(text.c_str(), 30);
        float centeredX = 78.0f + modeOptionX - textWidth / 2.0f;
        TextUtil::drawText(text, {centeredX, 430.0f}, 28, GRAY);
    }
}

    TextUtil::drawText("DIFFICULTY:", {diffTitleX, 280},30, col1_color);

    for (int i = 0; i < difficulties.size(); ++i) {
        Color color = (i == difficultyIndex) ? (Color){198, 128, 43, 255} : GRAY;
        float textWidth = MeasureText(difficulties[i], 28);
        float centeredX = 78.0f + diffOptionX - textWidth / 2.0f;
        TextUtil::drawText(difficulties[i], {centeredX, 340.0f + i * 50}, 28, color);
    }

    /*TextUtil::drawText("GAME MODE:", {250, 270}, 30, col0_color);
    for (int i = 0; i < modes.size(); ++i) {
        Color color = (i == modeIndex) ? (Color){198, 128, 43, 255} : WHITE;
        TextUtil::drawText(modes[i], {260, 320.0f + i * 50}, 28, color);
    }
    if (modeIndex == 1) {
        if (editingGoal) {
            for (int i = 0; i < sprint.size(); i++) {
                Color color = (i == sprintIndex) ? (Color){198, 128, 43, 255} : GRAY;
                TextUtil::drawText(std::to_string(sprint[i])+" ROWS", {260, 400.0f + i * 30}, 28, colPor);
            }
        }
        else {
            TextUtil::drawText(std::to_string(sprint[sprintIndex])+" ROWS", {260, 400.0f}, 28, GRAY);
        }
    }

    // Difficulty
    Color col1_color = (columnIndex == 1) ? (Color){198, 128, 43, 255} : WHITE;
    TextUtil::drawText("DIFFICULTY:", {550, 270}, 30, col1_color);
    for (int i = 0; i < difficulties.size(); ++i) {
        Color color = (i == difficultyIndex) ? (Color){198, 128, 43, 255} : WHITE;
        TextUtil::drawText(difficulties[i], {570, 320.0f + i * 50}, 28, color);
    }*/

    // Nickname (only in single mode)
    if (singleMode) {
        Color col2_color = (columnIndex == 2) ? (Color){198, 128, 43, 255} : WHITE;
        TextUtil::drawText("NICKNAME:", {880, 280}, 30, col2_color);
        Color color = (columnIndex == 2) ? (Color){198, 128, 43, 255} : WHITE;
        std::string displayName = editingNickname ? nickname + "_" : nickname;
        float nicknameWidth = MeasureTextEx(TextUtil::regularFont, "NICKNAME:", 30, 1.0f).x;
        float displayNameWidth = MeasureTextEx(TextUtil::regularFont, displayName.c_str(), 30, 1.0f).x;
        TextUtil::drawText(displayName, {880.0f - displayNameWidth/2.0f + nicknameWidth/2.0f, 340}, 30, color);
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