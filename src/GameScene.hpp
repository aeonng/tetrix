//gamescene.hpp
#pragma once
#include "Board.hpp"
#include "Block.hpp"
#include "Item.hpp"
#include "GameState.hpp"
#include <vector>
#include <string>

class GameScene {
public:
    GameScene(bool isSingle, int mode, int difficulty, int sprint_goal);
    GameState update();
    void render() const;

    float gameOverTime = 0;
    float gameOverTime2 = 0;

    int score = 0;
    int scoreP2 = 0;

    int num_clear_line = 0;
    int num_clear_lineP2 = 0;

    bool isSingleMode = true;
    int mode; // 0: Marathon, 1: Sprint
    int sprint_goal;
    int difficulty;

    float elapsedTime = 0;

private:
    // Player state helpers
    void updatePlayer(int playerId);
    bool isGameOver(int playerId);
    bool singleGameOver(int playerId);

    // UI display helpers
    void displayScore(int playerId) const;
    void displaySimultaneousClear(int p) const;
    void drawGhostBlock(int p) const;
    void displayNextBlock(int p) const;
    void displayCombo(int playerId) const;
    void displayClearedLines(int p) const;
    void displayElapsedTime() const;

    void BlockSwitch(int p);
    Block spawnBlock();
    int getNextBlockIdFrom7Bag();
    float update_speed_y(float elapsed_time);
    void handleInput(int playerId);

    // Game state variables
    Board board;
    Board boardP2;

    Block currentBlock;
    Block nextBlock;
    Block currentBlockP2;
    Block nextBlockP2;

    std::vector<int> blockBag;

    bool isPaused = false;

    int state = 0;
    int stateP2 = 0;

    float clearEffectTimerP1 = 0;
    float clearEffectTimerP2 = 0;
    float clearEffectDuration = 0.5f;

    float speed_y = 1.5f;
    float speed_y_P2 = 1.5f;

    int combo = 0;
    int comboP2 = 0;

    int cleared = 0;
    int clearedP2 = 0;

    int lastClearCount = 0;
    int lastClearCountP2 = 0;

    std::vector<int> clearedItems1;
    std::vector<int> clearedItems2;

    std::vector<int> lastClearedLines;
    std::vector<int> lastClearedLinesP2;

    bool wasBoardCleared = false;
    bool wasBoardClearedP2 = false;

    bool wasReplaced = false;
    bool wasReplacedP2 = false;

    bool gameover = false;
    bool gameoverP2 = false;

    bool isInput = false;
    bool isInputP2 = false;
};
