//board.cpp
#include "Board.hpp"
#include <raylib.h>
#include "Utility.hpp"
#include <iostream>
#include "GameScene.hpp"

Board::Board(int w, int h) : width(w), height(h), grid(h, std::vector<Grid>(w, Grid())), num_cleared_item(std::vector<int>(4)) {}

Board::Grid::Grid() : blockType(0), colorId(-1) {}
Board::Grid::Grid(int blockTyoe, int colorId) : blockType(blockTyoe), colorId(colorId) {}

bool Board::canPlaceBlock(const Block& block, int x, int y) const {
    const auto& shape = block.getShape();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (shape[i][j]) {
                int bx = x + j;
                int by = y + i;
                if (bx < 0 || bx >= width || by < 0 || by >= height) {
                    return false;
                }
                int c = grid[by][bx].colorId;
                int b = grid[by][bx].blockType;
                if ((b == 1 && c < 100) || (b == 2) || (b == -2)) 
                    return false;
            }
        }
    }
    return true;
}

void Board::placeBlock(const Block& block) { //issue
    const auto& shape = block.getShape();
    int x = block.x, y = getGhostY(block);
    //std::cout << "getghostY: " << y << std::endl;
    int b = block.blockId;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (shape[i][j] >= 1) {
                int bx = x + j;
                int by = y + i;
                grid[by][bx].blockType = shape[i][j];
                if (shape[i][j] == 1) {
                    grid[by][bx].colorId = b;
                }
                else if (shape[i][j] == 2) {
                    grid[by][bx].colorId = block.itemType;
                }
            }
        }
    }
}

bool Board::isFull(int y) {
    bool full = true;
    std::vector<int> temp_cleared_item;
    temp_cleared_item = num_cleared_item;
    for (int j = 0; j < width; ++j) {
        int c = grid[y][j].colorId;
        int b = grid[y][j].blockType;
        if (b >= 1) {
            if (c >= 100) {
                full = false;
                break;
            }
            if (b == 2) {
                temp_cleared_item[c]++;
            }
        }
        else if (b <= 0) {
            full = false;
            break;
        }
    }
    if (full) {
        num_cleared_item = temp_cleared_item;
    }
    return full;
}

int Board::clearFullLines() {
    lastClearedLines.clear();               // 이전 정보 초기화
    num_cleared_item = {0, 0, 0, 0};        // 아이템 효과도 초기화

    std::vector<int> rowsToClear;
    
    // 1. 삭제할 줄 인덱스 수집
    for (int i = 0; i < height; i++) {
        if (isFull(i)) {
            rowsToClear.push_back(i);
        }
    }

    // 2. 삭제 대상 줄 수 저장 (스코어용)
    int cleared = rowsToClear.size();

    // 3. 위에서 아래 순서로 줄 삭제
    for (int i = rowsToClear.size() - 1; i >= 0; i--) {
        int row = rowsToClear[i];
        lastClearedLines.push_back(row);
        grid.erase(grid.begin() + row);  // 해당 줄 제거

    }

    // 4. 줄 삭제 시 효과음
    if (cleared > 0) {
        AudioUtil::playDeleteSound();  
    }

    // 5. 맨 위에서부터 비어있는 줄 추가
    for (int i = 0; i < cleared; i++) {
        grid.insert(grid.begin(), std::vector<Grid>(width, Grid()));
    }

    return cleared;
}


std::vector<int> Board::getLastClearedLines() const {
    return lastClearedLines;
}

void Board::replaceBlocks() {
    for (int j = width -1; j >= 0; j--) {
        for (int i = height - 1; i >= 0; i--) {
            int b = grid[i][j].blockType;
            int c = grid[i][j].colorId;
            if (b > 0 && c < 100) {
                while (i+1 < height && grid[i+1][j].blockType == 0) {
                    grid[i+1][j].blockType = b;
                    grid[i+1][j].colorId = c;
                    grid[i][j] = Grid();
                    i++;
                }
                break;
            }
        }
    }
}

void Board::draw() const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int b = grid[i][j].blockType;
            if (b == 1) {
                DrawUtil::drawBlock(j, i, x_offset, y_offset, grid[i][j].colorId);
            }
            else if (b == 2) {
                DrawUtil::drawItem(j, i, x_offset, y_offset, grid[i][j].colorId);
            }
            else if (b == 0) {
                DrawUtil::drawFallbackCell(j, i, x_offset, y_offset, BLACK);
            }
            else if (b == -2) {
                DrawUtil::drawFallbackCell(j, i, x_offset, y_offset, GRAY); //수정 필요
            }
        }
    }
}

int Board::getGhostY(const Block& block) const {
    Block temp = block;
    temp.y += temp.y_offset / DrawUtil::CELL_SIZE;
    temp.y_offset = 0;

    while (canPlaceBlock(temp, temp.x, temp.y + 1)) {
        temp.y++;
    }
    return temp.y;
}

int Board::clearBottomLines(int count) {
    int grey_cleared = 0;
    count = std::min(count, height);

    for (int i = 0; i < count; ++i) {
        int y = height - 1 - i;
        if (grid[y][0].blockType == -2) grey_cleared++;
        // 줄 삭제
        grid.erase(grid.begin() + y);
    }

    // 삭제된 줄 수만큼 맨 위에 빈 줄 추가
    for (int i = 0; i < count; ++i) {
        grid.insert(grid.begin(), std::vector<Grid>(width, Grid()));
    }

    return grey_cleared;
}

void Board::addGreyRows(int count) {
    for (int i = 0; i < count; ++i) {
        grid.erase(grid.begin());
        grid.push_back(std::vector<Grid>(width, Grid(-2, -1)));  // grey block: colorId = -2, itemType = None
    }
}

void Board::addOpponentGreyRows(Board &opponent_board, int count) {
    for (int i = 0; i < count; ++i) {
        opponent_board.grid.erase(opponent_board.grid.begin());
        opponent_board.grid.push_back(std::vector<Grid>(width, Grid(-2, -1)));  // grey block: colorId = -2, itemType = None
    }
}

std::vector<int> Board::popClearedItems() {
    std::vector<int> result = num_cleared_item;
    num_cleared_item = std::vector<int>(4); // reset
    return result;
}
