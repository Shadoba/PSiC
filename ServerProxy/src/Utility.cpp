#include "Utility.hpp"

#include <sstream>
#include <iostream>

Utility::Utility()
{

}

std::vector<std::string> Utility::splitString(std::string input, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream stream(input);
    std::string token;

    while(std::getline(stream, token, delimiter))
    {
        result.push_back(token);
    }

    return result;
}

std::vector<std::string> Utility::splitString(std::string input, std::string delimiter)
{
    std::vector<std::string> result;

    size_t delimiterSize = delimiter.size();
    size_t position = input.find(delimiter);
    result.push_back(input.substr(0, position));
    while (position != std::string::npos)
    {
        size_t startPosition = position + delimiterSize;         //skip over the delimiter
        position = input.find(delimiter, startPosition);
        result.push_back(input.substr(startPosition, position - startPosition));
    }

    return result;
}

httpRequestMethod Utility::getMethodByString(const std::string input)
{
    if(!input.compare("CONNECT"))
        return httpRequestMethod::CONNECT;
    if(!input.compare("GET"))
        return httpRequestMethod::OTHER;
    return httpRequestMethod::INVALID;
}
protocol Utility::getProtocolByString(const std::string input)
{
    if(input.find("HTTP/") != std::string::npos)
        return protocol::HTTP;
    return protocol::INVALID;
}