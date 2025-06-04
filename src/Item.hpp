#pragma once
#include <vector>

class Board;

class Item {
private:
    int type; //NONE = -1, CLEAR_LINE_1 = 0, CLEAR_LINE_2 = 1,ADD_LINE = 2, OPPONENT_ADD_LINE = 3
    int x, y; //grid position

public:
    int num_item = 4;
    Item(int type, int x, int y);

    static void applyEffect(int type, Board& board);
    static int applyEffect(const std::vector<int>& items, Board& board);
    void draw() const;

    int getX() const;
    int getY() const;
    int getTypeId() const;

    static int chooseItemType(float elapsedTime, bool isSingle);
};
extern int total_grey_cleared;
