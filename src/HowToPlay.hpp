// HowToPlay.hpp
#pragma once
#include <string>
#include <vector>
#include "GameState.hpp"

class HowToPlay {
public:
    void draw() const;
    GameState update();

private:
    int currentPage = 0; //0: 첫번째 페이지, 1: 두번째 페이지
};
