// RankingPage.hpp
#pragma once
#include "ScoreSystem.hpp"  // ScoreEntry 정의 포함
#include <string>
#include <vector>
#include "GameState.hpp"

class RankingPage {
public:
    RankingPage();
    void draw() const;
    GameState update();

private:
    int mode; // 0 = Marathon, 1 = Sprint
    std::vector<ScoreSystem::ScoreEntry> entries;
    void loadScores();
};
