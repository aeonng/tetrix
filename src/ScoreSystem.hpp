// ScoreSystem.hpp
#pragma once
#include <string>
#include "GameScene.hpp"

namespace ScoreSystem {
    struct ScoreEntry {
    int score;
    int difficulty;
    float gameTime;
    };

    int calculateFinalScore(int baseScore, int difficulty, float gameTime, int mode, int sprintGoalAchieved);

    int writeScoreIfTop15(const ScoreEntry& entry, int mode); // returns ranking or -1

    std::string determineWinner(const GameScene& scene); // returns "P1" or "P2"
};