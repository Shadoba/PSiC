#include "HeadProcessor.hpp"

#include "Utility.hpp"

HeadProcessor::HeadProcessor(std::string head)
{
    m_head = head;

    std::vector<std::string> lines = Utility::splitString(head, '\n');

    std::vector<std::string> firstLineWords = Utility::splitString(lines.at(0), ' ');
    lines.erase(lines.begin());
    m_method = firstLineWords.at(0);
    m_url = firstLineWords.at(1);
    m_protocol = firstLineWords.at(2);

    for(unsigned int i = 0; i < lines.size(); i++)
    {
        std::vector<std::string> header = Utility::splitString(lines.at(i), ": ");
        m_headers.insert_or_assign(header.at(0), header.at(1));
    }
}

std::string HeadProcessor::getMethod()
{
    return m_method;
}

std::string HeadProcessor::getUrl()
{
    return m_url;
}

std::string HeadProcessor::getProtocol()
{
    return m_protocol;
}

std::map<std::string, std::string> HeadProcessor::getHeaders()
{
    return m_headers;
}

std::string HeadProcessor::getOutputHead()
{
    return m_outputHead;
}