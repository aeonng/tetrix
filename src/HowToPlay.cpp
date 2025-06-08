//// HowToPlay.cpp
#include "HowToPlay.hpp"
#include "Utility.hpp"
#include <fstream>
#include <sstream>
#include <raylib.h>


void HowToPlay::draw() const {
    auto it = DrawUtil::backgroundTextures.find(1); 
    if (it != DrawUtil::backgroundTextures.end()) {
        DrawTexture(it->second, 0, 0, WHITE);
    } else {
        ClearBackground(RAYWHITE);  
    }

    std::string title =  "How To Play";
    float titleWidth = MeasureText(title.c_str(), 40);
    TextUtil::drawText(title, {30+(1280 - titleWidth) / 2.0f, 200}, 45, GOLD);

    TextUtil::drawText("     PRESS H TO RETURN  ", {820, 575}, 21, LIGHTGRAY);
}

GameState HowToPlay::update() {
    if (IsKeyPressed(KEY_H)) {
        AudioUtil::playMoveSound();
        return GameState::START;
    }
    return GameState::HOW_TO_PLAY;
}