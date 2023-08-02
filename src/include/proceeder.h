//
// Created by user on 31.07.23.
//

#ifndef FAIL2DROP_PROCEEDER_H
#define FAIL2DROP_PROCEEDER_H
#include "inotify-cpp/NotifierBuilder.h"
#include "dropped.h"

namespace fail2drop {
    struct task {
        std::future<void> storage, notifier;
    };

    class proceeder {
    public:
        std::string logFile = "/var/log/auth.log";
        std::function<void(dropped& dr)> dropFunction = nullptr;

        [[nodiscard]] bool isAuthLogPresent() const;
        void makeWatcher();
        void run();
        void shutdown();
    private:
        static inline const int usernameStartOffset = 8;

        std::mutex sync = {};

        std::function<void(inotify::Notification notification)> onChanged = nullptr;
        inotify::NotifierBuilder builder = {};

        std::atomic_bool isShutdown = false;
        std::vector<dropped> droppedStorage;
        task droppedTask = {};

        void proceedLogLine(const std::string &line);
        void proceedDrop(const std::string& username, const std::string& ip, const std::string& port);
    };

}

#endif //FAIL2DROP_PROCEEDER_H
