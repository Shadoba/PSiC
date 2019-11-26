#pragma once

#include <vector>
#include <string>
#include <map>

class Utility
{
public:
    static std::vector<std::string> splitString(std::string input, char delimiter);
    static std::vector<std::string> splitString(std::string input, std::string delimiter);
    static httpRequestMethod getMethodByString(std::string input);
    static protocol getProtocolByString(std::string input);

private:
    Utility();
};

enum httpRequestMethod
{
    CONNECT,
    OTHER,
    INVALID
};

enum protocol
{
    HTTP,
    INVALID
};