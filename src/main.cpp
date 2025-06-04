//main.cpp
#include <raylib.h>
#include "GameManager.hpp"
#include "Utility.hpp"

int main() {
    InitWindow(1280, 720, "TETRIS");
    SetTargetFPS(60);

    InitAudioDevice();

    TextUtil::loadFonts();
    DrawUtil::loadBlockTextures();  
    DrawUtil::loadItemTextures();
    DrawUtil::loadBackgroundTextures();
    AudioUtil::loadSounds();

    GameManager gameManager;
    gameManager.run();

    TextUtil::unloadFonts();
    DrawUtil::unloadBlockTextures();
    DrawUtil::unloadItemTextures();
    DrawUtil::unloadBackgroundTextures();
    AudioUtil::unloadSounds();
    
    CloseAudioDevice();

    CloseWindow();
    return 0;
}