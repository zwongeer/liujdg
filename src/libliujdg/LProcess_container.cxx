#include <cstring>
#include <chrono>
#include <future>
#include <stdexcept>
#include <utility>

#include <boost/algorithm/string.hpp>

#include "Lfunc.hpp"
#include "LProcess.hpp"
#include "LProcess_container.hpp"

LProcess_container::LProcess_container():process(nullptr), limit(0) {}
LProcess_container::LProcess_container(const std::string& str, int lim, const std::string& currentDir):limit(lim){
    process = new LProcess(str, currentDir);
    eventList.clear();
}

bool LProcess_container::wait_for_getline(char* ret, int seconds) {
    if (seconds < 0) seconds = limit;
    if (!process) throw std::runtime_error("LProcess_container is not initialized");

    // to get the history messages
    if (!eventList.empty()) {
        std::future_status status = eventList.front()->wait_for(std::chrono::seconds(seconds));
        
        if (status == std::future_status::ready) {
            auto pair = eventList.front()->get();
            if (pair.second)
                throw std::runtime_error("String Length Limit Exceeded!");
            std::string retstr = std::move(pair.first);
            std::strcpy(ret, retstr.c_str());
            delete eventList.front();
            eventList.pop_front();
            return true;
        } else return false;
    }
    
    auto pfu =
        new std::future<std::pair<std::string, bool>> (
            std::async([&]()->std::pair<std::string, bool>{
                bool lengthLimitExceeded = false;
                mutex.lock();
                char* pstr = new char[LENGTHLIMIT + 1];
                size_t readSize = 0;
                char ch;
                while ( process->stdout_.read(&ch, 1) && ch != '\n') {
                    pstr[readSize++] = ch;
                    if (readSize == LENGTHLIMIT) {
                        lengthLimitExceeded = true;
                        mutex.unlock();
                        delete[] pstr;
                        return {"", false};
                    }
                }
                pstr[readSize] = '\0';
                std::string ret = pstr;
                boost::erase_all(ret, "\r");    
                mutex.unlock();
                delete[] pstr;
                return {ret, lengthLimitExceeded};
            })
        );
    
    std::future_status status = pfu->wait_for(std::chrono::seconds(seconds));

    if (status == std::future_status::ready) {
        auto pair = pfu->get();
        if (pair.second)
            throw std::runtime_error("String Length Limit Exceeded!");
        std::string retstr = std::move(pair.first);
        std::strcpy(ret, retstr.c_str());
        delete pfu;
        return true;
    }
    
    eventList.push_back(pfu);

    return false;
}


bool LProcess_container::hasNewMessage() {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");

    // to get the history messages
    if (!eventList.empty()) {
        std::future_status status = eventList.front()->wait_for(std::chrono::seconds(1));
        return status == std::future_status::ready;
    }
    
    auto pfu =
        new std::future<std::pair<std::string, bool>> (
            std::async([&]()->std::pair<std::string, bool>{
                bool lengthLimitExceeded = false;
                mutex.lock();
                char* pstr = new char[LENGTHLIMIT + 1];
                size_t readSize = 0;
                char ch;
                // while (::read(process->fd_from_child, &ch, 1) != 0 && ch != '\n') {
                while ( process->stdout_.read(&ch, 1) && ch != '\n') {
                    pstr[readSize++] = ch;
                    if (readSize == LENGTHLIMIT) {
                        lengthLimitExceeded = true;
                        mutex.unlock();
                        delete[] pstr;
                        return {"", false};
                    }
                }
                pstr[readSize] = '\0';
                std::string ret = pstr;
                boost::erase_all(ret, "\r");
                mutex.unlock();
                delete[] pstr;
                return {ret, lengthLimitExceeded};
            })
        );
    
    std::future_status status = pfu->wait_for(std::chrono::seconds(1));
    eventList.push_back(pfu);

    return status == std::future_status::ready;
}

LProcess_container::LProcess_container(LProcess_container&& rhs) {
    process = rhs.process;
    limit = rhs.limit;
    eventList = std::move(rhs.eventList);
    rhs.process = nullptr;
    rhs.limit = 0;
}

LProcess_container& LProcess_container::operator = (LProcess_container&& rhs) {
    delete process;
    process = rhs.process;
    limit = rhs.limit;
    eventList = std::move(rhs.eventList);
    rhs.process = nullptr;
    rhs.limit = 0;
    return *this;
}

bool LProcess_container::wait_or_kill(int lim) {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");

    auto fu = std::future<void> (std::async([&](){
        this->wait();
    }));
    std::future_status sta = fu.wait_for(std::chrono::seconds(lim));
    if (sta != std::future_status::ready) {
        process->kill();
        return false;
    }
    return true;
}

LProcess_container::~LProcess_container(){
    if (process != nullptr) process->kill();
    for (auto p : eventList)
        delete p;
    delete process;
}

int LProcess_container::getpid() {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");
    return process->getpid();
}

void LProcess_container::closeInPipe() {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");
    return process->closeInPipe();
}

void LProcess_container::wait() {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");
    return process->wait();
}
void LProcess_container::kill() {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");
    process->kill();
}

std::ostream& LProcess_container::getStdin() {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");
    // if (!isRunning()) throw std::runtime_error("LProcess_container is not running");
    // set bad bit instead of throw
    if (!isRunning())
        return liujdg::devNull;
    return process->getStdin();
}

std::istream& LProcess_container::getStdout() {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");
    return process->getStdout();
}

std::istream& LProcess_container::getStderr() {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");
    return process->getStderr();
}

std::optional<int> LProcess_container::getReturnValue() {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");
    return process->getReturnValue();
}

bool LProcess_container::isRunning() {
    if (!process) throw std::runtime_error("LProcess_container is not initialized");
    return process->isRunning();
}

void LProcess_container::lock() {
    mutex.lock();
}

void LProcess_container::unlock() {
    mutex.unlock();
}

void LProcess_container::waitEventList() {
    for (int i = 0; i < eventList.size(); ++i)
        delete eventList[i];
}

void LProcess_container::writeTo(std::string_view sv) {
    if (!isRunning()) return;
    // return ::write(process->fd_to_child, sv.data(), sv.size() / sizeof (char));
    process->getStdin().write(sv.data(), sv.size() / sizeof (char));
}
