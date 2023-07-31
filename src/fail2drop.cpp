//
// Created by user on 31.07.23.
//

#include <filesystem>
#include "include/fail2drop.h"
#include "spdlog/spdlog.h"
#include "include/util.h"

bool fail2drop::isAuthLogPresent() {
    return std::filesystem::exists(logFile);
}

void fail2drop::makeWatcher() {
    onChanged = [instance = this](const inotify::Notification &_) {
        std::string lastLogLine = util::getLastLineFromFile(logFile);

        if (lastLogLine.empty()) {
            spdlog::error("Last log line is empty");
        } else {
            instance->processLogLine(lastLogLine);
        }
    };

    builder = inotify::BuildNotifier().watchFile(logFile).onEvents({
        inotify::Event::modify
    }, onChanged);
}

void fail2drop::run() {
    builder.run();
}

void fail2drop::processLogLine(const std::string &line) {
    spdlog::debug("Got log line: {0}", line);
    if (line.find("Failed password for ") != std::string::npos) {
        auto tokens = util::split(line, ' ');

        int addOffset = line.find("Failed password for invalid user") != std::string::npos ? 2 : 0; //add 2 if invalid user

        //Username starts from offset 8
        //I guess username can contain space
        std::string username, ip, port;
        for (int i = usernameStartOffset + addOffset; i < tokens.size(); i++) {
            if (tokens.at(i) == "from" && util::isValidIp(tokens.at(i + 1))) {
                ip = tokens.at(i + 1);
                port = tokens.at(i + 3);
                break;
            }
            username += tokens.at(i);
        }

        spdlog::info("Got login from {0} username: {1}; ip: {2}; port: {3}", addOffset == 2 ? "non exist" : "exist", username, ip, port);
        processDrop(username, ip, port);
    }
}

void fail2drop::processDrop(const std::string &username, const std::string &ip, const std::string &port) const {
    if(dropFunction){
        dropFunction(username, ip, port);
    }
}
