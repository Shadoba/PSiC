#pragma once

#include <string>
#include <map>

class HeadProcessor
{
public:
    HeadProcessor(std::string head);

    std::string getMethod();
    std::string getUrl();
    std::string getProtocol();
    std::map<std::string, std::string> getHeaders();
    std::string getOutputHead();

private:
    void pullDataFromHead(std::string head);
    void transformHead(std::string head);

    std::string m_head;
    std::string m_method;
    std::string m_url;
    std::string m_protocol;
    std::map<std::string, std::string> m_headers;
    std::string m_outputHead;
};
