// EndScreen.cpp
#include "EndScreen.hpp"
#include "Utility.hpp"
#include "ScoreSystem.hpp"
#include <raylib.h>

EndScreen::EndScreen(const GameScene& scene) {
    isSingle = scene.isSingleMode;
    if (isSingle) {
        playerScore = scene.score;
        playTime = scene.elapsedTime;
        int sprintAchieved = (scene.mode == 1 && scene.num_clear_line >= scene.sprint_goal) ? 1 : 0;
        int finalScore = ScoreSystem::calculateFinalScore(playerScore, scene.difficulty, playTime, scene.mode, sprintAchieved);
        ranking = ScoreSystem::writeScoreIfTop15({finalScore, scene.difficulty, playTime}, scene.mode);
        newRecord = (ranking > 0);
        highestRecord = (ranking == 1);
    } else {
        winner = ScoreSystem::determineWinner(scene);
        score1 = scene.score;
        score2 = scene.scoreP2;
        totalTime = scene.elapsedTime;
    }
}

void EndScreen::render() const {
    ClearBackground(BLACK);
    if (isSingle) {
        TextUtil::drawText("Game Over", {200, 100}, 48, WHITE);
        TextUtil::drawText("Score: " + std::to_string(playerScore), {200, 160}, 32, LIGHTGRAY);
        TextUtil::drawText("Time: " + std::to_string(static_cast<int>(playTime)) + "s", {200, 200}, 32, LIGHTGRAY);

        if (newRecord) {
            std::string text = (highestRecord ? "New Highest Record!" : "New Record! Ranking #" + std::to_string(ranking));
            TextUtil::drawText(text, {200, 260}, 28, GOLD);
        }
    } else {
        TextUtil::drawText("The Winner is " + winner, {200, 100}, 48, WHITE);
        TextUtil::drawText("P1 Score: " + std::to_string(score1), {200, 160}, 28, LIGHTGRAY);
        TextUtil::drawText("P2 Score: " + std::to_string(score2), {200, 200}, 28, LIGHTGRAY);
        TextUtil::drawText("Time: " + std::to_string(static_cast<int>(totalTime)) + "s", {200, 240}, 28, LIGHTGRAY);
    }
}

bool EndScreen::update() {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_H)) return true; // go to main menu
    return false;
}