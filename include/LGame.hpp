#pragma once
#include <fstream>
#include <vector>

#include "LGameInfo.hpp"
#include "LGameResult.hpp"
#include "LProcess_container.hpp"

struct LGame {
    static constexpr const char* SEND = "send";
    static constexpr const char* WIN = "win";
    static constexpr const char* DRAW = "draw";
    static constexpr const char* CONTINUE = "continue";

    LGameInfo gameInfo;
    LGameResult gameResult;
    std::vector<LProcess_container> processes;
    std::vector<std::ofstream> outFiles; // store the stdout of all processes
    std::vector<int> peopleIndex; // random indexes for players
    std::vector<int> indexForJudger; // get the id of process from index
    bool isFinished;

    LGame() = default;
    LGame(const std::string& json);
    LGame(const LGame&) = delete;
    LGame(LGame&&) = default;
    LGame& operator = (const LGame&) = delete;
    LGame& operator = (LGame&&) = default;
    virtual ~LGame();
    
    void init();
    void run();
    void nextStep();
    bool hasDone();
    void writeResultToFile();
    bool isAllRunning();
    void wait();
};