#include <cctype>

#include <algorithm>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <unistd.h>

#include "Lfunc.hpp"
#include "LGame.hpp"

void LGame::fromString(const std::string& str) {
    gameInfo.fromString(str);
    // reserve space for indexes
    peopleIndex.resize(gameInfo.getPeopleNum());
    indexForJudger.resize(gameInfo.getPeopleNum());
    outFiles.resize(gameInfo.getPeopleNum());
    for (int i = 0; i < outFiles.size(); ++i) {
        if (gameInfo.config.logdir != "")
            outFiles[i].open(( gameInfo.config.logdir + std::to_string(i) + ".txt").c_str());
        else
            outFiles[i].open("/dev/null");
        if (!outFiles[i]) throw std::runtime_error("Cannot open the output files");
    }
    if (gameInfo.config.logdir != "") judger_logFile.open((gameInfo.config.logdir + "judger_log.txt").c_str());
    else judger_logFile.open("/dev/null");

    
    for (int i = 0; i < peopleIndex.size(); ++i)
        peopleIndex[i] = i;

    // generate random id for players
    std::random_shuffle(peopleIndex.begin() + 1, peopleIndex.end());

    // map the id
    for (int i = 0; i < peopleIndex.size(); ++i)
        indexForJudger[ peopleIndex[i] ] = i;
    
    for (int i = 1; i < peopleIndex.size(); ++i)
        judger_logFile << R"({"player_realid": )" << i << R"(, "random_id" : )" << peopleIndex[i] << "}\n";

    isFinished = false;
}

LGame::LGame(const std::string& json) {
    std::ifstream in(json.c_str());
    if (!in.good()) throw std::runtime_error(LINFO + "Cannot open the file `" + json + '`');
    std::string file = LreadFile(in);
    fromString(file);
}

void LGame::init() {
    std::string errMessage;
    
    // check directory permission
    if (gameInfo.config.basedir == ".")
        lpass();
    else if (LcheckDirectory(gameInfo.config.basedir, errMessage))
        chdir(gameInfo.config.basedir.c_str());
    else throw std::runtime_error(errMessage);

    if (!LcheckDirectory(gameInfo.config.logdir, errMessage)) throw std::runtime_error(errMessage);
}

void LGame::run() {
    // run the players and the judger
    // run command, time limit, path
    
    for (int i = 0; i < gameInfo.getPeopleNum(); ++i){
        processes.push_back(LProcess_container(gameInfo.persons[i].run, gameInfo.config.timeLimit, gameInfo.persons[i].basedir));
    }
    
    // passing the number of players to the judger (the index of the judger is 0)
    // 1.number of players (note that the playersNumber = peopleNumber - 1)
    // 2.path to store data (note that in the sandbox the path is the only folder that the process can access) 
                            //{BTW the sandbox is not implemented} 
    processes[0].stdin() << gameInfo.getPeopleNum() - 1 << std::endl;
    processes[0].stdin() << gameInfo.persons[0].folder << std::endl;

    // passing the number of players, the player id, and the folder to store data to players (2 lines)
    // 1.number of players
    // 2.id
    // 3.path
    for (int i = 1; i < gameInfo.getPeopleNum(); ++i) {
        processes[i].stdin() << gameInfo.getPeopleNum() - 1 << std::endl;
        processes[i].stdin() << peopleIndex[i] << std::endl;
        processes[i].stdin() << gameInfo.persons[i].folder << std::endl;
    }
}

void LGame::nextStep() {
    if (hasDone()) return;
    char str[4096];
    memset(str, 0, sizeof (str));
    bool flag;
    try {
        flag = processes[0].wait_for_getline(str);
    } catch (std::exception& err) {
        throw std::runtime_error(LINFO + "[judger]:" + std::string(err.what()));
    }
    
    if (!flag)
        throw std::runtime_error(LINFO + "Judger Time Limit Exceeded!");
    outFiles[0] << str << std::endl;

    std::string_view command = LgetCommand(str);
    int id, ret;
    std::vector<int> playerList;
    
    if (command == SEND) {
        std::stringstream ss;
        ss << str + std::strlen(SEND);

        // get send list
        playerList.clear();
        while (ss >> id)
            playerList.push_back(id);

        // get message to send
        try {
            flag = processes[0].wait_for_getline(str);
        } catch (std::exception& err) {
            throw std::runtime_error(LINFO + "[in judger]:" + std::string(err.what()));
        }
        if (!flag)
            throw std::runtime_error(LINFO + "Judger Time Limit Exceeded!");
        outFiles[0] << str << std::endl;

        // send the message (send to all players if id = 0)
        if (id != 0)
            for (int i = 0; i < playerList.size(); ++i)
                processes[ indexForJudger.at(playerList[i]) ].stdin() << str << std::endl;
        else if (playerList.size() == 1)
            for (int i = 1; i < gameInfo.getPeopleNum(); ++i)
                processes[i].stdin() << str << std::endl;
        else
            throw std::runtime_error(LINFO + "Judger `Send` Syntax Error");

    }
    else if (command == CONTINUE) {
        std::stringstream ss;
        ss << str + std::strlen(CONTINUE);
        ss >> id;

        // get message from player id
        try {
            flag = processes[indexForJudger.at(id)].wait_for_getline(str);
        } catch (std::exception& err) {
            throw std::runtime_error(LINFO + "in player[" + gameInfo.persons[indexForJudger.at(id)].name + "]:" + std::string(err.what()));
        }

        // check string is valid
        auto check = [](const std::string& s) -> bool {
            bool status = false;
            for (auto c : s) {
                if (::isprint(c) && !::isspace(c))
                    status = true;
            }
            return status;
        };
        if (!check(str)) throw std::runtime_error(LINFO + "in player[" + gameInfo.persons[indexForJudger.at(id)].name + "] No valid output (maybe process is exited)");

        if (!flag) throw std::runtime_error(LINFO + "in player[" + gameInfo.persons[indexForJudger.at(id)].name + "] Time Limit Exceeded!");
        outFiles[indexForJudger.at(id)] << str << std::endl;

        // send message to the judger
        // PLAYER_ID [MESSAGE]
        processes[0].stdin() << id << " " << str << std::endl;

    }
    else if (command == WIN) {
        std::stringstream ss;
        ss << str + std::strlen(WIN);

        // get win list
        playerList.clear();
        while (ss >> id)
            playerList.push_back(id);
        
        gameResult.playersResults.resize(gameInfo.getPeopleNum());

        // set the winner
        for (int i = 0; i < playerList.size(); ++i)
            gameResult.playersResults[ indexForJudger.at(playerList[i]) ].isWinner = true;

        // get the scores
        try {
            flag = processes[0].wait_for_getline(str);
        } catch (std::exception& err) {
            throw std::runtime_error(LINFO + "[in judger]:" + std::string(err.what()));
        }
        if (!flag)
            throw std::runtime_error(LINFO + "Judger Time Limit Exceeded!");
        outFiles[0] << str << std::endl;

        playerList.clear();

        ss = std::stringstream();
        ss << str;
        while (ss >> ret)
            playerList.push_back(ret);

        // set the scores
        if (playerList.size() != gameInfo.getPeopleNum() - 1) throw std::runtime_error(LINFO + "Judger did not score every player");
        for (int i = 0; i < playerList.size(); ++i)
            gameResult.playersResults[ indexForJudger.at(i + 1) ].scores = playerList[i];

        // get comments from the judger
        if (processes[0].hasNewMessage()) {
            try {
                flag = processes[0].wait_for_getline(str);
            } catch (std::exception& err) {
                throw std::runtime_error(LINFO + "[in judger]:" + std::string(err.what()));
            }
            if (!flag)
                throw std::runtime_error(LINFO + "Judger Time Limit Exceeded!");
            gameResult.comments = str;
            outFiles[0] << str << std::endl;
        } else gameResult.comments = "No comments";


        isFinished = true;
    }
    else if (command == DRAW) {
        std::stringstream ss;        
        gameResult.playersResults.resize(gameInfo.getPeopleNum());

        // get the scores
        try {
            flag = processes[0].wait_for_getline(str);
        } catch (std::exception& err) {
            throw std::runtime_error(LINFO + "[in judger]:" + std::string(err.what()));
        }
        if (!flag) 
            throw std::runtime_error(LINFO + "Judger Time Limit Exceeded!");
        outFiles[0] << str << std::endl;

        playerList.clear();
        ss = std::stringstream();
        ss << str;
        while (ss >> ret)
            playerList.push_back(ret);

        // set the scores
        if (playerList.size() != gameInfo.getPeopleNum() - 1) throw std::runtime_error(LINFO + "Judger did not score every player");
        for (int i = 0; i < playerList.size(); ++i)
            gameResult.playersResults[ indexForJudger.at(i + 1) ].scores = playerList[i];

        // get comments from the judger
        if (processes[0].hasNewMessage()) {
            try {
                flag = processes[0].wait_for_getline(str);
            } catch (std::exception& err) {
                throw std::runtime_error(LINFO + "[in judger]:" + std::string(err.what()));
            }
            if (!flag) {
                gameResult.comments = "";
            } else {
                gameResult.comments = str;
                outFiles[0] << str << std::endl;
            }
        } else gameResult.comments = "";


        isFinished = true;
    }
    else if (command == LOG) {
        try {
            flag = processes[0].wait_for_getline(str);
        } catch (std::exception& err) {
            throw std::runtime_error(LINFO + "[in judger]:" + std::string(err.what()));
        }
        if (!flag) 
            throw std::runtime_error(LINFO + "Judger Time Limit Exceeded!");
        outFiles[0] << str << std::endl; // sava the stdout of judger
        judger_logFile << str << std::endl; // judger's log
    }

}

bool LGame::hasDone() {
    if (isFinished) return true;
    // for (int i = 0; i < processes.size(); ++i)
    //     if (!processes[i].isRunning()) 
    //         throw std::runtime_error("The process of [" + gameInfo.persons[i].name + "," + gameInfo.persons[i].id + "] is not running");
    return false;
}

void LGame::writeResultToFile() {
    // set the name and if for players
    for (int i = 0; i < gameResult.playersResults.size(); ++i) {
        gameResult.playersResults[i].name = gameInfo.persons[i].name;
        gameResult.playersResults[i].id = gameInfo.persons[i].id;
    }

    gameResult.writeToFile(gameInfo.config.logdir + "result.json");
}

LGame::~LGame() {
    
}

void LGame::wait() {
    for (auto& v : processes)
        v.writeTo("#kill");
    auto start = std::chrono::steady_clock::now();
    decltype(start) now;
    while (now = std::chrono::steady_clock::now(),
        std::chrono::duration_cast<std::chrono::seconds>(now - start).count() <= gameInfo.config.timeLimit) {
        bool flag = false;
        for (auto& v : processes)
            if (v.isRunning()) flag = true;
        if (!flag) break;
    }
}

void LGame::waitAndKill() {
    this->wait();
    for (auto& v : processes)
        v.kill();
}

bool LGame::isAllRunning() {
    for (auto& v : processes)
        if (!v.isRunning()) return false;
    return true;
}