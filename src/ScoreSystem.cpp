// ScoreSystem.cpp
#include "ScoreSystem.hpp"
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

int ScoreSystem::calculateFinalScore(int baseScore, int difficulty, float gameTime, int mode, int sprintGoalAchieved) {
    float multiplier = (difficulty == 0) ? 1.0f : (difficulty == 1) ? 1.3f : 1.6f;
    int bonus = 0;
    if (mode == 0) bonus = static_cast<int>(1000.0f / (gameTime + 1.0f));
    else if (mode == 1 && sprintGoalAchieved) bonus = 5000;
    return static_cast<int>((baseScore + bonus) * multiplier);
}

int ScoreSystem::writeScoreIfTop15(const ScoreEntry& entry, int mode) {
    std::string filename = (mode == 0) ? "data/marathon.csv" : "data/sprint.csv";
    std::vector<ScoreEntry> entries;

    // load file
    std::ifstream infile(filename);
    if (infile) {
        int score, diff;
        float time;
        while (infile >> score >> diff >> time) {
            entries.push_back({score, diff, time});
        }
        infile.close();
    }

    entries.push_back(entry);
    std::sort(entries.begin(), entries.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
    });

    if (entries.size() > 15) entries.resize(15);

    // find ranking
    int rank = -1;
    for (int i = 0; i < entries.size(); ++i) {
        if (entries[i].score == entry.score && entries[i].gameTime == entry.gameTime) {
            rank = i + 1;
            break;
        }
    }

    std::ofstream outfile(filename);
    for (const auto& e : entries) {
        outfile << e.score << " " << e.difficulty << " " << e.gameTime << "\n";
    }
    return rank;
}

std::string ScoreSystem::determineWinner(const GameScene& scene) {
    if (scene.mode == 0) { // marathon
        if (scene.gameOverTime2 > scene.gameOverTime) return "P2";
        if (scene.gameOverTime2 < scene.gameOverTime) return "P1";
    } else if (scene.mode == 1) {
        if (scene.num_clear_lineP2 > scene.num_clear_line) return "P2";
        if (scene.num_clear_lineP2 < scene.num_clear_line) return "P1";
    }
    return (scene.scoreP2 > scene.score) ? "P2" : "P1";
}
