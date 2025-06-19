//gamemanager.cpp
#include "GameManager.hpp"
#include "Utility.hpp"
#include <raylib.h>

GameManager::GameManager() {
    state = GameState::START;
}

GameManager::~GameManager() {
    if (modeSelectPage) delete modeSelectPage;
    if (gameScene) delete gameScene;
    if (endScreen) delete endScreen;
    if (rankingPage) delete rankingPage;
    if (howToPlay) delete howToPlay;
}

void GameManager::run() {
    while (!WindowShouldClose()) {
        AudioUtil::updateMainSound();
        update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw();
        EndDrawing();
    }
}

void GameManager::update() {
    /*switch (state) {
        //case GameState::MODE_SELECT:
        case GameState::HOW_TO_PLAY:
        //case GameState::RANKING:
            if (IsKeyPressed(KEY_H)) {
                state = GameState::START;
                if (modeSelectPage) { delete modeSelectPage; modeSelectPage = nullptr; }
                if (gameScene) { delete gameScene; gameScene = nullptr; }
                return;
            }
            break;
    }*/

    switch (state) {

        case GameState::HOW_TO_PLAY:
            if (!howToPlay) howToPlay = new HowToPlay();  // 처음 한 번만 객체 생성
            state = howToPlay->update();  // HowToPlay 상태 업데이트
            break;

        case GameState::START:
            state = startPage.update();
            if (state == GameState::MODE_SELECT) {
                singlePlayerMode = startPage.getIsSinglePlayer();
                if (modeSelectPage) {
                    delete modeSelectPage;
                    modeSelectPage = nullptr;
                }
            }
            break;

        case GameState::MODE_SELECT:
            if (!modeSelectPage)
                modeSelectPage = new ModeSelectPage(singlePlayerMode);
            state = modeSelectPage->update();

            if (state == GameState::GAME_RUNNING) {
                mode = modeSelectPage->getModeIndex();
                difficulty = modeSelectPage->getDifficultyIndex();
                sprint_goal = modeSelectPage->getSprintGoal();
                nickname = modeSelectPage->getNickname();
                if (gameScene) delete gameScene;  // 먼저 기존 것이 있다면 삭제
                gameScene = new GameScene(singlePlayerMode, mode, difficulty, sprint_goal, nickname);  // 새로 할당
            }
            break;
        
        case GameState::RANKING:
            if (!rankingPage) rankingPage = new RankingPage();
            state = rankingPage -> update();
            break;
        
        case GameState::GAME_RUNNING:
            if (gameScene) state = gameScene->update();

            if (state == GameState::GAME_END) {
                if (endScreen) delete endScreen;
                endScreen = new EndScreen(*gameScene);
                delete gameScene;
                gameScene = nullptr;
            }
            break;
        
        case GameState::GAME_END:
            if (endScreen && endScreen->update()) {
                delete endScreen;
                endScreen = nullptr;
                state = GameState::START;
            }
            break;
        default:
            break;
    }
}

void GameManager::draw() {
    switch (state) {
        case GameState::START:
            startPage.draw();
            break;
        case GameState::MODE_SELECT:
            if (modeSelectPage) modeSelectPage->draw();
            break;
        case GameState::RANKING:
            if (rankingPage) rankingPage->draw();
            break;
        case GameState::GAME_RUNNING:
            if (gameScene) gameScene->render();
            break;
        case GameState::GAME_END:
            if (endScreen) endScreen -> render();
            break;
        case GameState::HOW_TO_PLAY:
            if (howToPlay) howToPlay -> draw();
            break;
        default:
            TextUtil::drawText("Next page not implemented", Vector2{400.0f, 400.0f}, 20, GRAY);
            break;
    }
}
