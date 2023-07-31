#include "src/include/fail2drop.h"
#include "spdlog/spdlog.h"
#include "src/include/util.h"

int main() {
    //spdlog::set_level(spdlog::level::debug);
    if(!util::isRoot()){
        spdlog::error("Must be spawned as root, can't continue");
    } else if(!fail2drop::isAuthLogPresent()) {
        spdlog::error("No auth.log file found, can't continue");
    } else {
        fail2drop dropper;
        dropper.dropFunction = [](const std::string &username, const std::string &ip, const std::string &port){
            system( fmt::format("iptables -I INPUT -s {} -j DROP", ip).data());
            spdlog::info("Blocked {0}", ip);
        };

        dropper.makeWatcher();
        dropper.run();
    }
    return 0;
}
