#pragma once

#include "GameState.hpp"
#include "StartPage.hpp"
#include "ModeSelectPage.hpp"
#include "GameScene.hpp"
#include "ScoreSystem.hpp"
#include "EndScreen.hpp"
#include "RankingPage.hpp"

class GameManager {
private:
    GameState state;
    StartPage startPage;
    ModeSelectPage* modeSelectPage = nullptr;
    GameScene* gameScene = nullptr;
    EndScreen* endScreen = nullptr;
    RankingPage* rankingPage = nullptr;
    
    bool singlePlayerMode = true;
    int mode;
    int difficulty;
    int sprint_goal;
    std::string nickname;

public:
    GameManager();
    ~GameManager();
    void run();
    void update();
    void draw();
};
