#pragma once
#include <raylib.h>
#include "GameState.hpp"

class StartPage {
private:
    int selectedIndex;
    bool isSinglePlayer = true;  // true=Single, false=Dual

    const char* menuItems[4] = {
        "HOW TO PLAY",
        "RANKINGS",
        "P PLAYER",
        "P PLAYER"
    };


public:
    StartPage();

    GameState update();
    void draw();
    bool getIsSinglePlayer() const { return isSinglePlayer; }  // 추가
};
