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

    // 추가: Sprint 모드용 정보
    int mode;           // mode 복사
    int sprintGoal;     // sprint goal
    int numClearLine;   // clear line 수

    // multi용
    std::string winner;
    int score1, score2;
    float totalTime;
};