//
// Created by user on 31.07.23.
//

#include <filesystem>
#include <future>
#include "include/proceeder.h"
#include "spdlog/spdlog.h"
#include "include/util.h"

bool fail2drop::proceeder::isAuthLogPresent() const {
    return std::filesystem::exists(logFile);
}

void fail2drop::proceeder::makeWatcher() {
    onChanged = [instance = this](const inotify::Notification &_) {
        std::string lastLogLine = util::getLastLineFromFile(instance->logFile);

        if (lastLogLine.empty()) {
            spdlog::error("Last log line is empty");
        } else {
            instance->proceedLogLine(lastLogLine);
        }
    };

    builder = inotify::BuildNotifier().watchFile(logFile).onEvents({
        inotify::Event::modify
    }, onChanged);
}

void fail2drop::proceeder::proceedLogLine(const std::string &line) {
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
        proceedDrop(username, ip, port);
    }
}

void fail2drop::proceeder::proceedDrop(const std::string& username, const std::string& ip, const std::string& port) {
    if(dropFunction){
        dropped dr(username, ip, port);
        dropFunction(dr);
        if(dr.getRunCommandAfterTime() > std::chrono::system_clock::now()) {
            std::scoped_lock<std::mutex> lock(sync);
            droppedStorage.emplace_back(std::move(dr));
        }
    }
}

void fail2drop::proceeder::run() {
    droppedTask.storage = std::async(std::launch::async, [instance = this](){
        spdlog::debug("Started storage");
       // std::scoped_lock<std::mutex> lock(instance->droppedTask.storageDone);
        while(!instance->isShutdown) {
            {
                std::scoped_lock<std::mutex> dlock(instance->sync);
                for (auto it(instance->droppedStorage.begin()); it != instance->droppedStorage.end();) {
                    if (std::chrono::system_clock::now() > it->getRunCommandAfterTime()) {
                        it->runCommand();
                        it = instance->droppedStorage.erase(it);
                        spdlog::debug("Deleted dropped from storage");
                    } else {
                        it++;
                    }
                }
            }

            sleep(1);
        }
    });

    droppedTask.notifier = std::async(std::launch::async, [instance = this]() {
        spdlog::debug("Started notifier");
        instance->builder.run();
    });
}

void fail2drop::proceeder::shutdown() {
    spdlog::debug("Shutdown");
    builder.stop();
    isShutdown = true;
    {
        droppedTask.storage.wait();
        droppedTask.notifier.wait();
    }
    spdlog::debug("Done");
}
