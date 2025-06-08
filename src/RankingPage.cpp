//rankingpage.cpp
#include "RankingPage.hpp"
#include "Utility.hpp"
#include <fstream>
#include <sstream>
#include <raylib.h>


RankingPage::RankingPage() : mode(0) {
    loadScores();
}

GameState RankingPage::update() {
    if (IsKeyPressed(KEY_LEFT)) {
        AudioUtil::playMoveSound();
        mode = (mode + 1) % 2;
        loadScores();
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        AudioUtil::playMoveSound();
        mode = (mode + 1) % 2;
        loadScores();
    }
    if (IsKeyPressed(KEY_H)) {
        AudioUtil::playMoveSound();
        return GameState::START;
    }
    return GameState::RANKING;
}


void RankingPage::loadScores() {
    entries.clear();
    std::string filename = (mode == 0) ? "data/marathon.csv" : "data/sprint.csv";
    std::ifstream infile(filename);
    if (!infile.is_open()) return;

    int score, difficulty;
    float time;
    while (infile >> score >> difficulty >> time) {
        entries.push_back({score, difficulty, time});
    }
}

void RankingPage::draw() const {
    auto it = DrawUtil::backgroundTextures.find(1); // 랭킹 배경 ID = 1
    if (it != DrawUtil::backgroundTextures.end()) {
        DrawTexture(it->second, 0, 0, WHITE);
    } else {
        ClearBackground(RAYWHITE);  
    }

    
    std::string title = (mode == 0) ? "Marathon Ranking" : "Sprint Ranking";
    float titleWidth = MeasureText(title.c_str(), 40);
    TextUtil::drawText(title, {30+(1280 - titleWidth) / 2.0f, 200}, 45, GOLD);

    
    TextUtil::drawText("PRESS <- OR -> TO SWITCH", {820, 550}, 21, LIGHTGRAY);
    TextUtil::drawText("     PRESS H TO RETURN  ", {820, 575}, 21, LIGHTGRAY);

    float startY = 270;
    int fontSize = 24;
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& e = entries[i];
        std::ostringstream oss;
        oss << i + 1 << " | SCORE: " << e.score
            << " | DIFF: " << (e.difficulty == 0 ? "EASY" : (e.difficulty == 1 ? "NORMAL" : "HARD"))
            << " | TIME: " << static_cast<int>(e.gameTime) << "s";
        TextUtil::drawText(oss.str(), {220, startY + static_cast<float>(i * 30)}, fontSize, RAYWHITE);
    }

    if (entries.empty()) {
        TextUtil::drawText("NO SCORES RECORDED YET.", {220, startY}, fontSize, LIGHTGRAY);
    }
}