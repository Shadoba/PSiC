#pragma once

#include <vector>
#include <string>

class Utility
{
public:
    static std::vector<std::string> splitString(std::string input, char delimiter);
    static std::vector<std::string> splitString(std::string input, std::string delimiter);
private:
    Utility();
};
