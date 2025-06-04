#pragma once
#include "GameScene.hpp"

class EndScreen {
public:
    EndScreen(const GameScene& scene);
    void render() const;
    bool update();

private:
    bool isSingle;
    int playerScore;
    float playTime;
    int ranking;
    bool newRecord;
    bool highestRecord;
    std::string winner;
    int score1, score2;
    float totalTime;
};