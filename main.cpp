#include <future>
#include "src/include/proceeder.h"
#include "spdlog/spdlog.h"
#include "src/include/util.h"

int main() {
    spdlog::set_level(spdlog::level::debug);
    fail2drop::proceeder dropper;
    //dropper.logFile = "/home/user/CLionProjects/fail2drop/ss.log";

    if(!util::isRoot()){
        spdlog::error("Must be spawned as root, can't continue");
    } else if(!dropper.isAuthLogPresent()) {
        spdlog::error("No auth.log file found, can't continue");
    } else {
        dropper.dropFunction = [](fail2drop::dropped& dropped){
            system(fmt::format("iptables -I INPUT -s {} -j DROP", std::get<1>(dropped.getCredentials())).data());
            spdlog::info("Blocked {0}", std::get<1>(dropped.getCredentials()));

            dropped.setTimeoutCommand(std::chrono::high_resolution_clock::now() + std::chrono::hours(5), [](fail2drop::dropped& dr){
                auto [user, ip, port] = dr.getCredentials();
                system(fmt::format("iptables -I INPUT -s {} -j ACCEPT", ip).data());
                spdlog::info("Unblocked {0}", ip);
            });
        };

        dropper.makeWatcher();
        dropper.run();
        std::promise<void>().get_future().wait(); //sleep forever
        dropper.shutdown();
    }
    return 0;
}
