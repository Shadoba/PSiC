#include "Utility.hpp"

#include <sstream>
#include <iostream>

std::vector<std::string> Utility::splitString(const std::string input, const char delimiter)
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

std::vector<std::string> Utility::splitString(const std::string input, std::string const delimiter)
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

std::vector<std::string> Utility::splitStringOnce(const std::string input, const std::string delimiter)
{
    std::vector<std::string> result;
    size_t delimiterSize = delimiter.size();
    size_t position = input.find(delimiter);
    result.push_back(input.substr(0, position));
    if(position == input.size() - delimiterSize)
    {
        result.push_back(std::string(""));
    }
    else
    {
        result.push_back(input.substr(position + delimiterSize));
    }
    return result;
}

std::string Utility::extractDomainName(std::string input)
{
    size_t start = 0, stop = 0;
    if(input.at(0) != 'w')
        start = input.find('w');
    stop = input.find('/', start);
    return input.substr(start, stop - start);
}

httpRequest::httpRequestMethod Utility::getMethodByString(const std::string input)
{
    if(!input.compare("CONNECT"))
        return httpRequest::httpRequestMethod::CONNECT;
    if(!input.compare("GET"))
        return httpRequest::httpRequestMethod::OTHER;
    return httpRequest::httpRequestMethod::INVALID;
}
protocol::protocol Utility::getProtocolByString(const std::string input)
{
    if(input.find("HTTP/") != std::string::npos)
        return protocol::protocol::HTTP;
    return protocol::protocol::INVALID;
}

const Utility::ConstableMap<std::string, httpRequest::httpRequestMethod> Utility::httpRequestMethodMap= 
{
    {"CONNECT", httpRequest::httpRequestMethod::CONNECT},
    {"GET", httpRequest::httpRequestMethod::OTHER},
};

const Utility::ConstableMap<std::string, protocol::protocol> Utility::protocolMap= 
{
    {"HTTP/1.0", protocol::protocol::HTTP},
    {"HTTP/1.1", protocol::protocol::HTTP},
    {"HTTP/2.0", protocol::protocol::HTTP},
};
