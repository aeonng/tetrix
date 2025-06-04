#pragma once
#include <string>
#include <array>
#include "GameState.hpp"

class ModeSelectPage {
private:
    int columnIndex;           // 0=mode, 1=difficulty, 2=nickname or 2=goal (if sprint)
    int modeIndex;             // 0=marathon, 1=sprint
    int difficultyIndex;       // 0=easy,1=normal,2=hard
    std::string nickname;
    int sprintIndex;            // sprint goal row count
    bool singleMode;
    bool isReady;
    bool editingNickname;
    bool editingGoal;

    const std::array<const char*, 2> modes = {"Marathon", "Sprint"};
    const std::array<const char*, 3> difficulties = {"Easy", "Normal", "Hard"};
    const std::array<const int, 4> sprint = {30, 50, 80, 100};

public:
    ModeSelectPage(bool single);
    GameState update();
    void draw();

    // getters
    std::string getNickname() const;
    int getModeIndex() const;
    int getDifficultyIndex() const;
    int getSprintGoal() const;
};