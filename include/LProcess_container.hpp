#pragma once

#include <deque>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

struct LProcess;

struct LProcess_container { 
    static constexpr int LENGTHLIMIT = 4096;

    LProcess* process;
    int limit;
    std::mutex mutex;
    std::deque<std::future<std::pair<std::string, bool>>*> eventList;

    virtual ~LProcess_container();
    LProcess_container();
    LProcess_container(const std::string& str, int lim = 3, const std::string& currentDir = ".");
    LProcess_container(const LProcess_container&) = delete;
    LProcess_container(LProcess_container&& );
    LProcess_container& operator = (LProcess_container&&);
    
    void writeTo(std::string_view sv);
    bool wait_for_getline(char* ret, int seconds = -1); // String Length Limit 4096
    bool hasNewMessage(); // the function wait 1 second to check whether there is a new message from stdout without extracting any content

    int getpid();
    void kill();
    void wait();
    std::ostream& getStdin();
    std::istream& getStdout();
    std::istream& getStderr();
    std::optional<int> getReturnValue();
    bool isRunning();
    void lock();
    void unlock();
    void closeInPipe();

    bool wait_or_kill(int lim); // kill the process if it doesn't exist in the time limit, return true if it exists normally

    void waitEventList(); // not implentmented
};
