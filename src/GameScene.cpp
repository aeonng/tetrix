// GameScene.cpp with full dual-mode support using offset-aware rendering
#include "GameScene.hpp"
#include "Utility.hpp"
#include <raylib.h>
#include "Board.hpp"
#include <iostream>
#include <random>
#include <algorithm>

std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, 6);

GameScene::GameScene(bool isSingle, int mode, int difficulty, int sprint_goal)
    : board(10, 20), boardP2(10, 20), isSingleMode(isSingle), mode(mode),
      sprint_goal(sprint_goal), difficulty(difficulty) {
    board.x_offset = isSingle ? ((1280-DrawUtil::CELL_SIZE*10)/2.0f): 100;
    boardP2.x_offset = 700; // P2 보드는 오른쪽으로 12칸 이동
    board.y_offset = 80;
    boardP2.y_offset = 80;
}

GameState GameScene::update() {
    if (IsKeyPressed(KEY_ENTER)) {
        isPaused = !isPaused;
    }

    if (isPaused) return GameState::GAME_END;

    elapsedTime += GetFrameTime();
    if (clearEffectTimerP1 > 0) clearEffectTimerP1 -= GetFrameTime();
    if (clearEffectTimerP2 > 0) clearEffectTimerP2 -= GetFrameTime();

    if (mode == 0) {
        if (!isSingleMode) {
            if (gameover || (!isSingleMode && gameoverP2)) {
                if (gameover) {gameOverTime = elapsedTime;}
                if (!isSingleMode && gameoverP2) {gameOverTime2 = elapsedTime;} 
                return GameState::GAME_END;
            }
        }
        else {
            if (gameover) {
                gameOverTime = elapsedTime;
                return GameState::GAME_END;
            }
        }
    }
    else if (mode == 1) {
        if (!isSingleMode) {
            if (gameover && (!isSingleMode && gameoverP2)) {
                return GameState::GAME_END;
            }
        }
        else {
            if (gameover) {
                return GameState::GAME_END;
            }
        }
    }
    if (!gameover) updatePlayer(1);
    if (!isSingleMode && !gameoverP2) {
        updatePlayer(2);
    }
    return GameState::GAME_RUNNING;
}

void GameScene::updatePlayer(int p) {
    Block &cur = (p == 1) ? currentBlock : currentBlockP2;
    Block &next = (p == 1) ? nextBlock : nextBlockP2;
    Board &b = (p == 1) ? board : boardP2;
    int &scoreRef = (p == 1) ? score : scoreP2;
    int &comboRef = (p == 1) ? combo : comboP2;
    int &clearedRef = (p == 1) ? cleared : clearedP2;
    int &lines = (p == 1) ? num_clear_line : num_clear_lineP2;
    bool &wasCleared = (p == 1) ? wasBoardCleared : wasBoardClearedP2;
    bool &wasReplacedRef = (p == 1) ? wasReplaced : wasReplacedP2;
    bool &gameOver = (p == 1) ? gameover : gameoverP2;
    std::vector<int> &clearedItems = (p == 1) ? clearedItems1 : clearedItems2;
    std::vector<int> &lastCleared = (p == 1) ? lastClearedLines : lastClearedLinesP2;
    int &lastClearCountRef = (p == 1) ? lastClearCount : lastClearCountP2;
    int &stateRef = (p == 1) ? state : stateP2;
    float &speedY = (p == 1) ? speed_y : speed_y_P2;
    bool &isInputRef = (p == 1) ? isInput : isInputP2;
    float &clearTimerRef = (p == 1) ? clearEffectTimerP1 : clearEffectTimerP2;

    switch (stateRef) {
        case 0: {
            cur = spawnBlock();
            next = spawnBlock();
            stateRef = 1;
            break;
        }
        case 1: {
            //std::cout << "state 1\n";
            if ((stateRef == 1) && !b.canPlaceBlock(cur, cur.x, cur.y_offset / DrawUtil::CELL_SIZE + 1)) {
                b.placeBlock(cur);
                stateRef = 2;
                isInputRef = false;
                break;
            }
            speedY = update_speed_y(elapsedTime);
            handleInput(p);
            if (!isInputRef) cur.y_offset += speedY;
            isInputRef = false;
            break;
        }
        case 2: {
            b.num_cleared_item = {0, 0, 0, 0};
            clearedRef = b.clearFullLines();
            lastClearCountRef = clearedRef;
            if (clearedRef > 0) {
                std::cout << "lastClearCountRef: " << lastClearCountRef << std::endl;
                if (wasCleared) comboRef++;
                lines += clearedRef;
                scoreRef += 100 * clearedRef + 100 * comboRef;
                clearedItems = b.popClearedItems();
                lastCleared = b.getLastClearedLines();
                clearTimerRef = clearEffectDuration;
                stateRef = 3;
            } else {
                if (!wasReplacedRef) comboRef = 0;
                stateRef = 5;
            }
            wasCleared = (clearedRef > 0);
            break;
        }
        case 3: {
            //std::cout << "state 3\n";
            int num_item_3 = clearedItems[3];
            if (!isSingleMode && num_item_3 > 0) {
                if (p == 1) {
                    board.addOpponentGreyRows(boardP2, num_item_3);
                }
                else {
                    boardP2.addOpponentGreyRows(board, num_item_3);
                }
            }
            int numGrey = Item::applyEffect(clearedItems, b);
            clearedRef += numGrey;
            scoreRef += 100 * numGrey;
            clearedItems.clear();
            stateRef = 4;
            break;
        }
        case 4: {
            //std::cout << "state 4\n";
            b.replaceBlocks();
            wasReplacedRef = true;
            stateRef = 2;
            break;
        }
        case 5: {
            //std::cout << "state 5\n";
            cur = next;
            next = spawnBlock();
            gameOver = isGameOver(p);
            stateRef = 1;
            wasReplacedRef = false;
            break;
        }
    }
}

void GameScene::render() const {
    board.draw();
    if (!isSingleMode) boardP2.draw();

    displayNextBlock(1);
    if (!isSingleMode) displayNextBlock(2);

    displayScore(1);
    if (!isSingleMode) displayScore(2);

    displayElapsedTime();

    if (isPaused) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 180});
        const char* msg = "PAUSED";
        int fontSize = 48;
        int textWidth = MeasureText(msg, fontSize);
        DrawText(msg, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2 - fontSize / 2, fontSize, RAYWHITE);
        return;
    }

    if (state == 1) {
        drawGhostBlock(1);
    }

    if (!isSingleMode && stateP2 == 1)  {
        drawGhostBlock(2);
    }

    currentBlock.draw(false, board);
    if (!isSingleMode) {
        currentBlockP2.draw(false, boardP2);
    }

    for (int p : {1, 2}) {
        if (p == 2 && isSingleMode) continue;
        const auto& lines = (p == 1) ? lastClearedLines : lastClearedLinesP2;
        float x_offset = (p == 1) ? board.x_offset : boardP2.x_offset;
        float y_offset = (p == 1) ? board.y_offset : boardP2.y_offset;
        float timer = (p == 1) ? clearEffectTimerP1 : clearEffectTimerP2;
        if (0 < timer) {
            if (timer > 0.25f) {
                for (int y : lines) {
                    for (int x = 0; x < 10; ++x) {
                        Color c = ((clearEffectDuration- timer) < 0.08f || (clearEffectDuration- timer) > 0.16f) ? RAYWHITE : WHITE;
                        DrawUtil::drawFallbackCell(x, y, x_offset, y_offset, c);
                    }
                }
            }
        displaySimultaneousClear(1);
        if (!isSingleMode) displaySimultaneousClear(2);
    
        displayCombo(1);
        if (!isSingleMode) displayCombo(2);
        }
    }

    if (mode == 1) {
        displayClearedLines(1);
        if (!isSingleMode) displayClearedLines(2);
    }

    displayCombo(1);
    if (!isSingleMode) displayCombo(2);
}

bool GameScene::isGameOver(int p) {
    if (mode == 1) {
        if ((p == 1 && num_clear_line >= sprint_goal) || (p == 2 && num_clear_lineP2 >= sprint_goal)) {
            return true;
        }
    }
    return singleGameOver(p);
}

bool GameScene::singleGameOver(int p) {
    Block &block = (p == 1) ? currentBlock : currentBlockP2;
    Board &b = (p == 1) ? board : boardP2;
    Block indicator = block;
    return !b.canPlaceBlock(indicator, indicator.x, b.getGhostY(indicator));
}

void GameScene::drawGhostBlock(int p) const {
    const Block& cur = (p == 1) ? currentBlock : currentBlockP2;
    const Board& b = (p == 1) ? board : boardP2;
    Block ghost = cur;
    int gy = b.getGhostY(cur);
    ghost.setPosition(cur.x, gy);
    ghost.y_offset = 0;
    std::cout << "p: " << p << " cur.x: " << cur.x << " gy: " << gy << std::endl;
    ghost.draw(true, b);
}

void GameScene::displayNextBlock(int p) const {
    const Block& next = (p == 1) ? nextBlock : nextBlockP2;
    float offsetX = (p == 1) ? board.x_offset + 330 : boardP2.x_offset + 330;
    float offsetY = (p == 1) ? board.y_offset + 200 : boardP2.y_offset + 200;

    TextUtil::drawText("NEXT", {offsetX, offsetY - 30}, 24, DARKBLUE);

    const auto& shape = next.getShape();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int cell = shape[i][j];
            if (cell == 1) {
                DrawUtil::drawBlock(j, i, offsetX, offsetY, next.blockId);
            } else if (cell == 2) {
                DrawUtil::drawItem(j, i, offsetX, offsetY, next.getItemType());
            }
        }
    }
}

void GameScene::displayScore(int p) const {
    int scoreVal = (p == 1) ? score : scoreP2;
    float x_offset = (p == 1) ? board.x_offset : boardP2.x_offset;
    float y_offset = (p == 1) ? board.y_offset : boardP2.y_offset;

    std::string scoreText = "SCORE: " + std::to_string(scoreVal);
    Vector2 pos = { x_offset + 330.0f, y_offset + 80.0f };
    TextUtil::drawText(scoreText, pos, 24, DARKGRAY);
}

void GameScene::displaySimultaneousClear(int p) const {
    const std::vector<int>& lines = (p == 1) ? lastClearedLines : lastClearedLinesP2;
    int count = (p == 1) ? lastClearCount : lastClearCountP2;
    if (lines.empty() || count == 0) return;

    std::cout << "lastclearcount display: " << count << std::endl;
    static const std::vector<std::string> clearTextList = {
        "Single +100", "Double +200", "Triple +300", "Quadra +400"
    };
    if (count < 1 || count > 4) return;

    std::string text = clearTextList[count - 1];
    std::cout << text << std::endl;
    int y = lines.back();
    float fontSize = 24;
    int textWidth = MeasureText(text.c_str(), fontSize);
    float boardX = (p == 1 ? board.x_offset : boardP2.x_offset);
    float boardY = (p == 1 ? board.y_offset : boardP2.y_offset);
    float posX = boardX + (10 * DrawUtil::CELL_SIZE - textWidth) / 2.0f;
    float posY = y * DrawUtil::CELL_SIZE + boardY - fontSize - 5;

    TextUtil::drawText(text, {posX, posY}, fontSize, WHITE);
}

void GameScene::displayCombo(int p) const {
    const std::vector<int>& lines = (p == 1) ? lastClearedLines : lastClearedLinesP2;
    int comboVal = (p == 1) ? combo : comboP2;
    if (comboVal < 2 || lines.empty()) return;

    std::string text = "COMBO X" + std::to_string(comboVal);
    std::cout << text << std::endl;
    int y = lines.front();
    float fontSize = 32;
    int textWidth = MeasureText(text.c_str(), fontSize);
    float boardX = (p == 1 ? board.x_offset : boardP2.x_offset);
    float boardY = (p == 1 ? board.y_offset : boardP2.y_offset);
    float posX = boardX + (10*DrawUtil::CELL_SIZE - textWidth) / 2.0f;
    float posY = y * DrawUtil::CELL_SIZE + boardY - fontSize - 28.0;
    TextUtil::drawText(text, {posX, posY}, fontSize, YELLOW);
}

Block GameScene::spawnBlock() {
    Block block;

    block = Block(getNextBlockIdFrom7Bag());
    block.setPosition(3, 0);

    int type = Item::chooseItemType(elapsedTime, isSingleMode); // dual 여부 전달
    block.setItemType(type);
    block.setItemPos(type != -1);

    int initialRot = rng() % 4;
    Block::Type block_type = block.getTypeFromId(block.blockId);
    auto [cx, cy] = Block::rotationCenters.at(block_type);
    for (int i = 0; i < initialRot; ++i) {
        block.shape = block.rotateShapeWithCenter(block.shape, cx, cy);
    }
    return block;
}

int GameScene::getNextBlockIdFrom7Bag() {
    if (blockBag.empty()) {
        blockBag = {0, 1, 2, 3, 4, 5, 6};
        std::shuffle(blockBag.begin(), blockBag.end(), rng);
    }
    int id = blockBag.back();
    blockBag.pop_back();
    return id;
}

void GameScene::displayClearedLines(int p) const {
    int lines = (p == 1) ? num_clear_line : num_clear_lineP2;
    float x_offset = (p == 1) ? board.x_offset : boardP2.x_offset;
    float y_offset = board.y_offset;

    std::string clearText = "LINES: " + std::to_string(lines);
    int fontSize = 32;
    Vector2 pos = { x_offset + 330.0f, y_offset };

    TextUtil::drawText(clearText, pos, fontSize, DARKBLUE);
}

void GameScene::displayElapsedTime() const {
    int seconds = static_cast<int>(elapsedTime);
    int minutes = seconds / 60;
    seconds %= 60;
    int fontSize = 24;

    std::string timeText = (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    float textWidth = MeasureText(timeText.c_str(), fontSize);
    TextUtil::drawText(timeText, {(GetScreenWidth() - textWidth)/2.0f, 10}, fontSize, BLACK);
}

void GameScene::BlockSwitch(int p) {
    Block &cur = (p == 1) ? currentBlock : currentBlockP2;
    Block &next = (p == 1) ? nextBlock : nextBlockP2;
    std::swap(cur, next);
    next.setPosition(3, 0);
    next.y_offset = 0;
}

float GameScene::update_speed_y(float elapsed_time) {
    return 1.5 + difficulty * 0.7 + int(elapsed_time/60) * 0.2f;
}
    
void GameScene::handleInput(int p) {
    Block &cur = (p == 1) ? currentBlock : currentBlockP2;
    Board &b = (p == 1) ? board : boardP2;
    bool &isInputRef = (p == 1) ? isInput : isInputP2;
    float &speedYRef = (p == 1) ? speed_y : speed_y_P2;

    if (IsKeyPressed(p == 1 ? KEY_SLASH : KEY_R)) {
        isInputRef = true;
        b.placeBlock(cur);
        if (p == 1) state = 2;
        else stateP2 = 2;
    }
    if (IsKeyPressed(p == 1 ? KEY_LEFT : KEY_F)) {
        isInputRef = true;
        cur.x -= 1;
        if (!b.canPlaceBlock(cur, cur.x, (cur.y_offset + DrawUtil::CELL_SIZE - 1) / DrawUtil::CELL_SIZE))
            cur.x += 1;
    }
    if (IsKeyPressed(p == 1 ? KEY_RIGHT : KEY_H)) {
        isInputRef = true;
        cur.x += 1;
        if (!b.canPlaceBlock(cur, cur.x, (cur.y_offset + DrawUtil::CELL_SIZE - 1) / DrawUtil::CELL_SIZE))
            cur.x -= 1;
    }
    if (IsKeyDown(p == 1 ? KEY_DOWN : KEY_G)) {
        speedYRef *= 5;
    }
    if (IsKeyPressed(p == 1 ? KEY_K : KEY_ONE)) {
        isInputRef = true;
        cur.attemptRotateCounterClockwise(b);
    }
    if (IsKeyPressed(p == 1 ? KEY_L : KEY_TWO)) {
        isInputRef = true;
        cur.attemptRotateClockwise(b);
    }
    if (IsKeyPressed(p == 1 ? KEY_SEMICOLON : KEY_THREE)) {
        isInputRef = true;
        BlockSwitch(p);
    }
}