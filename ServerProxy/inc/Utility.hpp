#pragma once

#include <vector>
#include <string>
#include <map>

enum httpRequestMethod
{
    CONNECT = 0u,
    OTHER,
    INVALID = 255u
};

enum protocol
{
    HTTP = 0u,
    INVALID = 255u
};

class Utility
{
public:
    static std::vector<std::string> splitString(std::string input, char delimiter);
    static std::vector<std::string> splitString(std::string input, std::string delimiter);
    static httpRequestMethod getMethodByString(std::string input);
    static protocol getProtocolByString(std::string input);
    Utility() = delete;
};