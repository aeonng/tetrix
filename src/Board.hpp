//board.hpp
#pragma once
#include <vector>
#include "Block.hpp"
#include "Item.hpp"

class Board {
private:
    int width, height;

public:
    class Grid {
    public:
        Grid();
        Grid(int blockType, int colorId);
        int colorId; //default: -1, normal_block: 0~6, itemblock: 0~3 (ghost_block > 100)
        int blockType; //grey_row = -2, default = 0, normal_block/ghost_block = 1, itemblock = 2
    };

    float x_offset;
    float y_offset;
    std::vector<int> lastClearedLines;
    std::vector<std::vector<Grid>> grid;
    Board(int width, int height);
    std::vector<int> num_cleared_item; //num_cleared_item[0] = 3: 0번 아이템 효과 3번 적용

    bool canPlaceBlock(const Block& block, int x, int y) const;
    void placeBlock(const Block& block);
    bool isFull(int y); //yth row가 가득 찾는가
    int clearFullLines(); //clear 된 줄 안에 item이 있을 경우, apply item effect
    std::vector<int> getLastClearedLines() const;
    void replaceBlocks(); //줄이 clear 된 후 공중에 있는 block들이 아래로 내려오도록.

    void draw() const;
    int getGhostY(const Block& block) const;

    int clearBottomLines(int count);   // 아이템: 밑줄 삭제
    void addGreyRows(int count);        // 아이템: grey 줄 추가
    void addOpponentGreyRows(Board &opponent_board, int count);

    std::vector<int> popClearedItems();
};
