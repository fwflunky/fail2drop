//
// Created by user on 31.07.23.
//

#include "include/util.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fstream>

std::string util::getLastLineFromFile(const std::string &filename) {
    std::fstream file(filename);
    std::string lastLine;

    if(file.is_open()) {
        file.seekg(0, std::ios::end);
        while(true)
        {
            file.unget();
            file.unget();
            if(file.get() == '\n'){
                std::getline(file, lastLine);
                break;
            }
        }
    }
    return lastLine;
}

std::vector<std::string> util::split(const std::string &str, char delim) {
    std::vector<std::string> out;
    boost::split(out, str, boost::is_any_of(&delim));
    return out;
}

bool util::isValidIp(const std::string &str) {
    return split(str, '.').size() == 4;
}

bool util::isRoot() {
    return getuid() == 0;
}
