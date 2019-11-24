#include "Utility.hpp"

#include <sstream>

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

    size_t inputSize = input.size();
    size_t delimiterSize = delimiter.size();
    size_t position = input.find(delimiter);
    result.push_back(input.substr(0, position));
    
    while (position != std::string::npos)
    {
        size_t previousPosition = position + delimiterSize;         //skip over the delimiter
        position = input.find(delimiter, previousPosition);
        result.push_back(input.substr(previousPosition, position));
    }

    return result;
}