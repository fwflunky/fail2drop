//
// Created by user on 31.07.23.
//

#ifndef FAIL2DROP_UTIL_H
#define FAIL2DROP_UTIL_H


#include <string>
#include <vector>

class util {
public:
    static std::string getLastLineFromFile(const std::string& filename);
    static std::vector<std::string> split(const std::string& str, char delim);
    static bool isValidIp(const std::string& str);
    static bool isRoot();
};


#endif //FAIL2DROP_UTIL_H
