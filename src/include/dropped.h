//
// Created by user on 31.07.23.
//

#ifndef FAIL2DROP_DROPPED_H
#define FAIL2DROP_DROPPED_H

#include <tuple>
#include <string>
#include <functional>
#include <thread>

namespace fail2drop {
    class dropped {
    public:
        dropped(std::string  username, std::string  ip, std::string  port);

        std::tuple<std::string, std::string, std::string> getCredentials();
        void setTimeoutCommand(std::chrono::time_point<std::chrono::system_clock> timeout, std::function<void(dropped&)> cmd);
        [[nodiscard]] std::chrono::time_point<std::chrono::system_clock> getRunCommandAfterTime() const;
        void runCommand();
    private:
        std::string username, ip, port;

        std::chrono::time_point<std::chrono::system_clock> runCommandAfter = {}; //todo use chrono
        std::function<void(dropped& self)> runnableCommand = nullptr;
    };
}
#endif //FAIL2DROP_DROPPED_H
