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
    std::string name;
    while (infile >> score >> difficulty >> time >> name) {
        entries.push_back({score, difficulty, time, name});
    }
}

void RankingPage::draw() const {

    if (mode == 0)
    {
        auto it = DrawUtil::backgroundTextures.find(12); // 랭킹 배경 ID = 12
        if (it != DrawUtil::backgroundTextures.end()) {
            DrawTexture(it->second, 0, 0, WHITE);
        } else {
            ClearBackground(RAYWHITE);  
        }
    } else {
        auto it = DrawUtil::backgroundTextures.find(13); // 랭킹 배경 ID = 13
        if (it != DrawUtil::backgroundTextures.end()) {
            DrawTexture(it->second, 0, 0, WHITE);
        } else {
            ClearBackground(RAYWHITE);  
        }
    }

    float startY = 260;
    int fontSize = 32;

    // 각 열의 X 좌표 지정
    float xRank = 305;
    float xName = 420;
    float xScore = 585;
    float xLevel = 740;
    float xTime = 915;

    size_t maxDisplay = std::min(entries.size(), size_t(8));

    for (size_t i = 0; i < maxDisplay; ++i) {
        const auto& e = entries[i];
        float y = startY + static_cast<float>(i * 30);

        // 랭크 
        TextUtil::numText(std::to_string(i + 1), {xRank, y}, 30, RAYWHITE);

        // 닉네임
        TextUtil::drawText(e.nickname, {xName, y}, fontSize, RAYWHITE);

        // 점수
        TextUtil::numText(std::to_string(e.score), {xScore, y}, 25, RAYWHITE);

        // 난이도
        std::string levelStr = (e.difficulty == 0 ? "Easy" : (e.difficulty == 1 ? "Normal" : "Hard"));
        TextUtil::drawText(levelStr, {xLevel, y}, fontSize, RAYWHITE);

        // 시간
        TextUtil::numText(std::to_string(static_cast<int>(e.gameTime)) + "s", {xTime, y}, 25, RAYWHITE);
    }
    
    /*float startY = 250;
    int fontSize = 32;
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& e = entries[i];
        std::ostringstream oss;
        oss << i + 1 << "    " << e.nickname
            << "    " << e.score
            << "    " << (e.difficulty == 0 ? "EASY" : (e.difficulty == 1 ? "NORMAL" : "HARD"))
            << "    " << static_cast<int>(e.gameTime) << "s";
        TextUtil::drawText(oss.str(), {280, startY + static_cast<float>(i * 30)}, fontSize, RAYWHITE);
    }

    if (entries.empty()) {
        TextUtil::drawText("NO SCORES RECORDED YET.", {220, startY}, fontSize, LIGHTGRAY);
    }*/
}