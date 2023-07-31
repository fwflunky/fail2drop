//
// Created by user on 31.07.23.
//
#include <utility>
#include "include/dropped.h"

fail2drop::dropped::dropped(std::string username, std::string ip, std::string port) : username(std::move(username)), ip(std::move(ip)), port(std::move(port)) {

}

std::tuple<std::string, std::string, std::string> fail2drop::dropped::getCredentials() {
    return {username, ip, port};
}

void fail2drop::dropped::setTimeoutCommand(std::chrono::time_point<std::chrono::system_clock> timeout, std::function<void(dropped&)> cmd) {
    runCommandAfter = timeout;
    runnableCommand = std::move(cmd);
}

std::chrono::time_point<std::chrono::system_clock> fail2drop::dropped::getRunCommandAfterTime() const {
    return runCommandAfter;
}

void fail2drop::dropped::runCommand() {
    runnableCommand(*this);
}



