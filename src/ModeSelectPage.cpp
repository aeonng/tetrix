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
            editingNickname = !editingNickname;
        }
    } 
    else if (editingGoal && columnIndex == 0 && modeIndex == 1) {
        if (IsKeyPressed(KEY_DOWN)) {
            sprintIndex = (sprintIndex + 1) % 4;
        }
        else if (IsKeyPressed(KEY_UP)) {
            sprintIndex = (sprintIndex - 1 + 4) % 4;
        }
        else if (IsKeyPressed(KEY_ENTER)) {
            editingGoal = !editingGoal;
        }
    }
    else {
        if (IsKeyPressed(KEY_LEFT)) columnIndex = (columnIndex - 1 + (singleMode ? 3 : 2)) % (singleMode ? 3 : 2);
        if (IsKeyPressed(KEY_RIGHT)) columnIndex = (columnIndex + 1) % (singleMode ? 3 : 2);

        if (IsKeyPressed(KEY_UP)) {
            if (columnIndex == 0) modeIndex = (modeIndex - 1 + 2) % 2;
            else if (columnIndex == 1) difficultyIndex = (difficultyIndex - 1 + 3) % 3;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            if (columnIndex == 0) modeIndex = (modeIndex + 1) % 2;
            else if (columnIndex == 1) difficultyIndex = (difficultyIndex + 1) % 3;
        }

        if (singleMode && columnIndex == 2) {
            if (IsKeyPressed(KEY_ENTER)) {
                editingNickname = !editingNickname;
            }
        }

        if (columnIndex == 0 && modeIndex == 1) {  
            if (IsKeyPressed(KEY_ENTER)) {
                editingGoal = !editingGoal;
            }
        }

        if (IsKeyPressed(KEY_H)) {
            return GameState::START;
        }
    }

    if (IsKeyPressed(KEY_S) && !editingNickname && !editingGoal && (!singleMode || (singleMode && !nickname.empty()))) {
            isReady = true;
            return GameState::GAME_RUNNING;
    }

    return GameState::MODE_SELECT;
}

void ModeSelectPage::draw() {
    ClearBackground(RAYWHITE);
    TextUtil::drawTextTitle(singleMode ? "Single Player Setup" : "Dual Player Setup", {400, 80}, 40, BLACK);
    TextUtil::drawText("Press 'H' to return HOME", {20, 680}, 20, BLACK);
    TextUtil::drawText("Press 'S' to START", {20, 650}, 20, BLACK);

    // Mode
    Color col0_color = (columnIndex == 0) ? BLUE : BLACK;
    TextUtil::drawText("Game Mode:", {200, 200}, 30, col0_color);
    for (int i = 0; i < modes.size(); ++i) {
        Color color = (i == modeIndex) ? RED : GRAY;
        TextUtil::drawText(modes[i], {220, 240.0f + i * 40}, 28, color);
    }
    if (modeIndex == 1) {
        if (editingGoal) {
            for (int i = 0; i < sprint.size(); i++) {
                Color color = (i == sprintIndex) ? RED : GRAY;
                TextUtil::drawText(std::to_string(sprint[i])+" ROWS", {220, 310.0f + i * 30}, 28, color);
            }
        }
        else {
            TextUtil::drawText(std::to_string(sprint[sprintIndex])+" ROWS", {220, 310.0f}, 28, GRAY);
        }
    }

    // Difficulty
    Color col1_color = (columnIndex == 1) ? BLUE : BLACK;
    TextUtil::drawText("Difficulty:", {550, 200}, 30, col1_color);
    for (int i = 0; i < difficulties.size(); ++i) {
        Color color = (i == difficultyIndex) ? RED : GRAY;
        TextUtil::drawText(difficulties[i], {570, 240.0f + i * 40}, 28, color);
    }

    // Nickname (only in single mode)
    if (singleMode) {
        Color col2_color = (columnIndex == 2) ? BLUE : BLACK;
        TextUtil::drawText("Nickname:", {900, 200}, 30, col2_color);
        Color color = (columnIndex == 2) ? RED : GRAY;
        std::string displayName = editingNickname ? nickname + "_" : nickname;
        TextUtil::drawText(displayName, {920, 250}, 28, color);
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