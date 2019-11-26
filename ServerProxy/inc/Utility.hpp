#pragma once

#include <vector>
#include <string>

class Utility
{
public:
    static std::vector<std::string> splitString(const std::string input, const char delimiter);
    static std::vector<std::string> splitString(const std::string input, const std::string delimiter);
    Utility() = delete;
};
