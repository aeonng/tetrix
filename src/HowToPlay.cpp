//// HowToPlay.cpp
#include "HowToPlay.hpp"
#include "Utility.hpp"
#include <fstream>
#include <sstream>
#include <raylib.h>


void HowToPlay::draw() const {
    auto it = DrawUtil::backgroundTextures.find(currentPage == 0 ? 3 : 4); 
    if (it != DrawUtil::backgroundTextures.end()) {
        DrawTexture(it->second, 0, 0, WHITE);
    } else {
        ClearBackground(RAYWHITE);  
    }
}

GameState HowToPlay::update() {
    if (IsKeyPressed(KEY_H)) {
        AudioUtil::playMoveSound();
        return GameState::START;
    }

    if (IsKeyPressed(KEY_RIGHT)) {
        AudioUtil::playMoveSound();
        currentPage = 1;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        AudioUtil::playMoveSound();
        currentPage = 0;
    }

    return GameState::HOW_TO_PLAY;
}