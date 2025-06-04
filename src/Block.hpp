//block.hpp
#pragma once
#include <array>
#include <utility>
#include <vector>
#include <map>

class Board;

class Block {
public:
    enum class Type { I, O, T, S, Z, J, L };
    static Type getTypeFromId(int id);
    using Shape = std::array<std::array<int, 4>, 4>;
    static const std::map<Type, std::pair<int, int>> rotationCenters;

private:
    //int state = -1; //0: block queue, 1: next block to be placed, 2: block being placed, 3: block placed
    static const std::map<Type, std::map<std::pair<int, int>, std::vector<std::pair<int, int>>>> kickTable;

public:
    Shape shape;
    int rotation;
    int x; //grid
    int y; //grid 
    float y_offset = 0; //pixel
    int blockId;
    int itemType;
    Block();
    Block(int blockId); //blockId가 곧 colorId
    Block(Shape shape);

    Shape rotateShapeWithCenter(const Shape& s, int cx, int cy) const;
    Shape rotateShapeCCWWithCenter(const Shape& s, int cx, int cy) const;
    bool attemptRotateClockwise(Board& board);
    bool attemptRotateCounterClockwise(Board& board);

    const Shape& getShape() const;
    void setPosition(int x, int y);
    int getColorId() const;

    bool hasItem() const;
    int getItemType() const;
    void setItemType(int type);

    void setItemPos(bool isItemIncluded);
    void draw(bool isGhost, Board board) const;

};

extern const std::vector<Block::Shape> tetrominoShapes;