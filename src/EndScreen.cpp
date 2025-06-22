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

        // sprint 관련 값 복사
        mode = scene.mode;
        sprintGoal = scene.sprint_goal;
        numClearLine = scene.num_clear_line;

        sprintAchieved = (scene.mode == 1 && scene.num_clear_line >= scene.sprint_goal) ? 1 : 0;
        int finalScore = ScoreSystem::calculateFinalScore(playerScore, scene.difficulty, playTime, scene.mode, sprintAchieved);
        //ranking = ScoreSystem::writeScoreIfTop15({finalScore, scene.difficulty, playTime}, scene.mode);
        ScoreSystem::ScoreEntry entry = {
            finalScore,
            scene.difficulty,
            playTime,
            scene.nickname  // <-- nickname도 함께 저장
        };
        ranking = ScoreSystem::writeScoreIfTop15(entry, scene.mode);
        
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
        //marathon 모드
        if (mode == 0) {
            auto it = DrawUtil::backgroundTextures.find(9); 
            if (it != DrawUtil::backgroundTextures.end()) {
                DrawTexture(it->second, 0, 0, WHITE);
            } else {
                ClearBackground(RAYWHITE);  
            }
            TextUtil::drawText(std::to_string(playerScore), {633, 570}, 35, WHITE);
            TextUtil::drawText(std::to_string(static_cast<int>(playTime)) + "s", {633, 525}, 35, WHITE);
        } else {
            //sprint 모드
            if (sprintAchieved == 1) {
                auto it = DrawUtil::backgroundTextures.find(10); 
                if (it != DrawUtil::backgroundTextures.end()) {
                    DrawTexture(it->second, 0, 0, WHITE);
                } else {
                    ClearBackground(RAYWHITE);  
                }
            } else {
                auto it = DrawUtil::backgroundTextures.find(11); 
                if (it != DrawUtil::backgroundTextures.end()) {
                    DrawTexture(it->second, 0, 0, WHITE);
                } else {
                    ClearBackground(RAYWHITE);  
                }
            }
            TextUtil::drawText(std::to_string(playerScore), {633, 570}, 35, WHITE);
            TextUtil::drawText(std::to_string(static_cast<int>(playTime)) + "s", {633, 525}, 35, WHITE);
            std::string lineText = std::to_string(numClearLine) + " / " + std::to_string(sprintGoal);
            TextUtil::drawText(lineText, {633, 615}, 35, LIGHTGRAY);
        }
        
        /*
        // Sprint 모드일 때 clear line 표시
        if (mode == 1) {
            std::string lineText = "Lines Cleared: " + std::to_string(numClearLine) + " / " + std::to_string(sprintGoal);
            TextUtil::drawText(lineText, {200, 240}, 32, LIGHTGRAY);
        }*/

        if (newRecord) {
            std::string text = (highestRecord ? "   New Highest Record!  " : "New Record! Ranking #" + std::to_string(ranking));
            TextUtil::drawText(text, {490, 660}, 28, (Color){198, 128, 43, 255});
        }
    } else {
        if (winner == "P1"){
            auto it = DrawUtil::backgroundTextures.find(7); 
            if (it != DrawUtil::backgroundTextures.end()) {
                DrawTexture(it->second, 0, 0, WHITE);
            } else {
                ClearBackground(RAYWHITE);  
            }
        } else {
            auto it = DrawUtil::backgroundTextures.find(8); 
            if (it != DrawUtil::backgroundTextures.end()) {
                DrawTexture(it->second, 0, 0, WHITE);
            } else {
                ClearBackground(RAYWHITE);  
            }
        }
        TextUtil::drawText(std::to_string(score1), {655, 570}, 35, WHITE);
        TextUtil::drawText(std::to_string(score2), {655, 615}, 35, WHITE);
        TextUtil::drawText(std::to_string(static_cast<int>(totalTime)) + "s", {633, 525}, 35, WHITE);
    }
}

bool EndScreen::update() {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_H)) return true; // go to main menu
    return false;
}