#include "SRS.hpp"
#include <iostream>

namespace SRS {
    bool testMode = false;
}

// 테스트할 blockId 리스트 (I, T, L, J 등)
static const std::vector<int> testBlockIds = { 2, 5, 0 };  // 예: J, T, L 블럭

// 테스트 위치
static const std::vector<int> xPositions = { 0, 3, 7 };
static const int yBase = 16;

void placeObstacle(Board& board) {
    // 중간 부분에 고정 장애물 설치
    for (int x = 4; x <= 5; ++x) {
        board.placeBlock(Block(0));  // Dummy block to fill grid
        board.grid[18][x].blockType = 1;
        board.grid[18][x].colorId = 1;
    }
}

void SRS::runRotationTest(Board& board) {
    std::cout << "==== [SRS TEST STARTED] ====\n";

    // 장애물 블럭 배치
    placeObstacle(board);

    for (int id : testBlockIds) {
        for (int x : xPositions) {
            Block b(id);
            b.setPosition(x, yBase);

            std::cout << "Testing BlockID " << id << " at (" << x << "," << yBase << ")\n";

            // CW 회전
            std::cout << "01\n";
            if (b.attemptRotateClockwise(board)) {
                std::cout << "kick found ✅ CW Rotation Success. New Pos: (" << b.x << "," << b.y << ") rot: " << b.rotation << "\n";
            } else {
                std::cout << "❌ CW Rotation Failed.\n";
            }

            // CCW 회전
            std::cout << "03\n";
            if (b.attemptRotateCounterClockwise(board)) {
                std::cout << "kick found ✅ CCW Rotation Success. New Pos: (" << b.x << "," << b.y << ") rot: " << b.rotation << "\n";
            } else {
                std::cout << "❌ CCW Rotation Failed.\n";
            }

            // 회전 결과를 board에 직접 반영
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    if (b.getShape()[i][j])
                        board.grid[b.y + i][b.x + j] = Board::Grid(b.getShape()[i][j], b.blockId);
        }
    }

    std::cout << "==== [SRS TEST ENDED] ====\n";
    SRS::testMode = false;
}