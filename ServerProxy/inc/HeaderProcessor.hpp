#pragma once

#include <string>
#include <map>

#include "Utility.hpp"

class HeaderProcessor
{
public:
    HeaderProcessor(std::string head);

    std::string getMethod();
    std::string getUrl();
    std::string getUri();
    std::string getProtocol();
    std::map<std::string, std::string> getFields();
    std::string getOutputHead();

private:
    void pullDataFromHead(std::string head);
    void transformHead(std::string head);
    void calculateUri();

    std::string m_head;
    std::string m_method;
    std::string m_url;
    std::string m_uri;
    std::string m_protocol;
    std::map<std::string, std::string> m_fields;
    std::string m_outputHeader;
};
