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
        mode = (mode + 1) % 2;
        loadScores();
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        mode = (mode + 1) % 2;
        loadScores();
    }
    if (IsKeyPressed(KEY_H)) {
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
    ClearBackground(DARKBLUE);

    std::string title = (mode == 0) ? "Marathon Ranking" : "Sprint Ranking";
    TextUtil::drawText(title, {100, 50}, 40, YELLOW);
    TextUtil::drawText("Press <- or -> to switch", {100, 100}, 20, LIGHTGRAY);
    TextUtil::drawText("Press H to return", {100, 130}, 20, LIGHTGRAY);

    float startY = 180;
    int fontSize = 24;
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& e = entries[i];
        std::ostringstream oss;
        oss << i + 1 << ". Score: " << e.score
            << " | Diff: " << (e.difficulty == 0 ? "Easy" : (e.difficulty == 1 ? "Normal" : "Hard"))
            << " | Time: " << static_cast<int>(e.gameTime) << "s";
        TextUtil::drawText(oss.str(), {100, startY + static_cast<float>(i * 30)}, fontSize, RAYWHITE);
    }

    if (entries.empty()) {
        TextUtil::drawText("No scores recorded yet.", {100, startY}, fontSize, LIGHTGRAY);
    }
}
