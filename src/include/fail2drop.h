//
// Created by user on 31.07.23.
//

#ifndef FAIL2DROP_FAIL2DROP_H
#define FAIL2DROP_FAIL2DROP_H
#include "inotify-cpp/NotifierBuilder.h"
class fail2drop {
public:
    static inline std::string logFile = "/var/log/auth.log";
    static bool isAuthLogPresent();

    std::function<void(const std::string& username, const std::string& ip, const std::string& port)> dropFunction = nullptr;

    void makeWatcher();
    void run();
private:
    static inline const int usernameStartOffset = 8;

    std::function<void(inotify::Notification notification)> onChanged = nullptr;
    inotify::NotifierBuilder builder = {};

    void processLogLine(const std::string& line);
    void processDrop(const std::string& username, const std::string& ip, const std::string& port) const;
};


#endif //FAIL2DROP_FAIL2DROP_H
