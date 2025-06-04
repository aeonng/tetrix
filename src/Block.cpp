//block.hpp
#include "Block.hpp"
#include "Utility.hpp"
#include "Board.hpp"
#include <raylib.h>
#include <random>
#include <algorithm>
#include <iostream>

const std::vector<Block::Shape> tetrominoShapes = {
    // I
    {{
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }},
    // O
    {{
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }},
    // T
    {{
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }},
    // S
    {{
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }},
    // Z
    {{
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }},
    // J
    {{
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }},
    // L
    {{
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }}
};

Block::Type Block::getTypeFromId(int id) {
    switch (id) {
        case 0: return Type::I;
        case 1: return Type::O;
        case 2: return Type::T;
        case 3: return Type::S;
        case 4: return Type::Z;
        case 5: return Type::J;
        case 6: return Type::L;
    }
}

const std::map<Block::Type, std::pair<int, int>> Block::rotationCenters = {
    { Type::I, {1, 2} },
    { Type::O, {1, 1} },
    { Type::T, {1, 1} },
    { Type::S, {1, 1} },
    { Type::Z, {1, 1} },
    { Type::J, {1, 1} },
    { Type::L, {1, 1} },
};

const std::map<Block::Type, std::map<std::pair<int, int>, std::vector<std::pair<int, int>>>> Block::kickTable = {
    { Type::I, {
        {{0, 1}, {{0,0}, {-2,0}, {1,0}, {-2,1}, {1,-2}}},
        {{1, 0}, {{0,0}, {2,0}, {-1,0}, {2,-1}, {-1,2}}},
        {{1, 2}, {{0,0}, {-1,0}, {2,0}, {-1,-2}, {2,1}}},
        {{2, 1}, {{0,0}, {1,0}, {-2,0}, {1,2}, {-2,-1}}},
        {{2, 3}, {{0,0}, {2,0}, {-1,0}, {2,-1}, {-1,2}}},
        {{3, 2}, {{0,0}, {-2,0}, {1,0}, {-2,1}, {1,-2}}},
        {{3, 0}, {{0,0}, {1,0}, {-2,0}, {1,2}, {-2,-1}}},
        {{0, 3}, {{0,0}, {-1,0}, {2,0}, {-1,-2}, {2,1}}},
    }},
    { Type::T, {
        {{0,1}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},
        {{1,0}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}},
        {{1,2}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}},
        {{2,1}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},
        {{2,3}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},
        {{3,2}, {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}},
        {{3,0}, {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}},
        {{0,3}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},
    }},
    { Type::J, {
        {{0,1}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},
        {{1,0}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}},
        {{1,2}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}},
        {{2,1}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},
        {{2,3}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},
        {{3,2}, {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}},
        {{3,0}, {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}},
        {{0,3}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},
    }},
    { Type::L, {
        {{0,1}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},
        {{1,0}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}},
        {{1,2}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}},
        {{2,1}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},
        {{2,3}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},
        {{3,2}, {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}},
        {{3,0}, {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}},
        {{0,3}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},
    }},
    { Type::S, {
        {{0,1}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},
        {{1,0}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}},
        {{1,2}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}},
        {{2,1}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},
        {{2,3}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},
        {{3,2}, {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}},
        {{3,0}, {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}},
        {{0,3}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},
    }},
    { Type::Z, {
        {{0,1}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},
        {{1,0}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}},
        {{1,2}, {{0,0},{1,0},{1,-1},{0,2},{1,2}}},
        {{2,1}, {{0,0},{-1,0},{-1,1},{0,-2},{-1,-2}}},
        {{2,3}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},
        {{3,2}, {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}},
        {{3,0}, {{0,0},{-1,0},{-1,-1},{0,2},{-1,2}}},
        {{0,3}, {{0,0},{1,0},{1,1},{0,-2},{1,-2}}},
    }}
};


Block::Block() : blockId(0), rotation(0), x(0), y(0), itemType(-1), shape(tetrominoShapes[0]) {}

Block::Block(int blockId) : rotation(0), x(0), y(0), itemType(-1), blockId(blockId), shape(tetrominoShapes[blockId]) {}

Block::Block(Shape shape) : rotation(0), x(0), y(0), itemType(-1), blockId(0), shape(shape) {}

Block::Shape Block::rotateShapeWithCenter(const Shape& s, int cx, int cy) const {
    std::vector<std::tuple<int, int, int>> blocks;  // (y, x, value)

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (s[i][j] != 0) {
                int rel_i = i - cy;
                int rel_j = j - cx;
                int ni = cx - rel_i;
                int nj = cy + rel_j;
                blocks.emplace_back(nj, ni, s[i][j]);
            }
        }
    }

    int minY = 0, minX = 0;
    if (blockId == 0) { 
        minY = 4, minX = 4;
        for (auto& [y, x, _] : blocks) {
            minY = std::min(minY, y);
            minX = std::min(minX, x);
        }
    }

    Shape ret = {{
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }};
    for (auto& [y, x, val] : blocks) {
        int sy = y - minY;
        int sx = x - minX;
        if (sy >= 0 && sy < 4 && sx >= 0 && sx < 4) ret[sy][sx] = val;
    }

    return ret;
}

Block::Shape Block::rotateShapeCCWWithCenter(const Shape& s, int cx, int cy) const {
    std::vector<std::tuple<int, int, int>> blocks;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (s[i][j] != 0) {
                int rel_i = i - cy;
                int rel_j = j - cx;
                // 반시계 회전: (x, y) → (-y, x)
                int ni = cx + rel_i;
                int nj = cy - rel_j;
                blocks.emplace_back(nj, ni, s[i][j]);
            }
        }
    }

    int minY = 0, minX = 0;
    if (blockId == 0) { 
        minY = 4, minX = 4;
        for (auto& [y, x, _] : blocks) {
            minY = std::min(minY, y);
            minX = std::min(minX, x);
        }
    }

    Shape ret = {{
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }};
    for (auto& [y, x, val] : blocks) {
        int sy = y - minY;
        int sx = x - minX;
        if (sy >= 0 && sy < 4 && sx >= 0 && sx < 4) ret[sy][sx] = val;
    }

    return ret;
}

bool Block::attemptRotateClockwise(Board& board) {
    Type type = getTypeFromId(blockId);
    auto [cx, cy] = rotationCenters.at(type);
    Shape newShape = rotateShapeWithCenter(shape, cx, cy);
    rotation = (rotation % 4);
    int newRot = (rotation + 1) % 4;
    std::cout << rotation << newRot << std::endl;
    if (type == Type::O) {
        shape = newShape;
        rotation = newRot;
        return true;
    }
    auto kicks = kickTable.at(type).at({rotation, newRot});
    std::cout << "kick found";
    for (auto [dx, dy] : kicks) {
        if (board.canPlaceBlock(Block(newShape), x + dx, (y_offset+DrawUtil::CELL_SIZE-1)/DrawUtil::CELL_SIZE+ dy)) {
            if (dx != 0 || dy != 0) std::cout << "SRS";
            shape = newShape;
            x += dx; 
            y += dy;
            rotation = newRot;
            return true;
        }
    }
    return false;
}

bool Block::attemptRotateCounterClockwise(Board& board) {
    Type type = getTypeFromId(blockId);
    auto [cx, cy] = rotationCenters.at(type);
    Shape newShape = rotateShapeCCWWithCenter(shape, cx, cy);
    rotation = rotation % 4;
    int newRot = (rotation + 3) % 4;
    std::cout << rotation << newRot << std::endl;
    if (type == Type::O) {
        shape = newShape;
        rotation = newRot;
        return true;
    }
    auto kicks = kickTable.at(type).at({rotation, newRot});
    std::cout << "kick found";
    for (auto [dx, dy] : kicks) {
        if (board.canPlaceBlock(Block(newShape), x + dx, (y_offset+DrawUtil::CELL_SIZE-1)/DrawUtil::CELL_SIZE+ dy)) {
            shape = newShape;
            x += dx;
            y += dy;
            rotation = newRot;
            return true;
        }
    }
    return false;
}

const Block::Shape& Block::getShape() const { return shape; }
void Block::setPosition(int nx, int ny) { x = nx; y = ny; }
int Block::getItemType() const { return itemType; }
bool Block::hasItem() const { return itemType != -1; }

void Block::setItemType(int type) { itemType = type; }

void Block::setItemPos(bool isItemIncluded) {
    if (isItemIncluded) {
        int r = rand() % 4;
        int item_index = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (shape[i][j] == 1) {
                    if (item_index == r) {
                        shape[i][j] = 2;
                        return;
                    }
                    item_index ++;
                }
            }
        }
    }
}

void Block::draw(bool isGhost, Board board) const {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int gx = x + j, gy = y + i;
            int block_c = isGhost ? blockId + 100 : blockId;
            int item_c = isGhost ? itemType + 100 : itemType;
            if (shape[i][j] == 1) {
                DrawUtil::drawBlock(gx, gy, board.x_offset, board.y_offset + y_offset, block_c);
            }
            else if (shape[i][j] == 2)  {
                DrawUtil::drawItem(gx, gy, board.x_offset, board.y_offset + y_offset, item_c);
            }
        }
    }
}