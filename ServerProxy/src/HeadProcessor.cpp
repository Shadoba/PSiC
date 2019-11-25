#include "HeadProcessor.hpp"

#include "Utility.hpp"

#include <iostream>

HeadProcessor::HeadProcessor(std::string head)
{
    m_head = head;
    pullDataFromHead(head);
    if(m_method.compare(std::string("CONNECT")))        //0 to równe, czyli wchodzi jeśli nie "CONNECT"
        transformHead(head);
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

void HeadProcessor::pullDataFromHead(std::string head)
{
    std::vector<std::string> lines = Utility::splitString(head, "\r\n");
    std::vector<std::string> firstLineWords = Utility::splitString(lines.at(0), ' ');
    lines.erase(lines.begin());
    m_method = firstLineWords.at(0);
    m_url = firstLineWords.at(1);
    m_protocol = firstLineWords.at(2);

    for(unsigned int i = 0; i < lines.size(); i++)
    {
        if(!lines.at(i).empty())
        {
            std::vector<std::string> header = Utility::splitString(lines.at(i), ": ");
            m_headers.insert(std::pair<std::string, std::string>(header.at(0), header.at(1)));
        }
    }
}

//Na razie tylko przepisuje i podmienia URL, jeśli trzeba
void HeadProcessor::transformHead(std::string head)
{
    m_outputHead += m_method;

    size_t position = m_url.find('/');
    if(position != 0)                                       //Jeśli pierwszym znakiem nie jest '/', to mamy pełen URL
    {
        position = m_url.find('/', position + 2);
        if(position == std::string::npos)                   //Jeśli poza "http://" nie ma '/', to musimy dodać sam '/'
            m_outputHead += " /";
        m_outputHead += " " + m_url.substr(position);
    }
    m_outputHead += " " + m_protocol + "\r\n";
    
    for(std::map<std::string, std::string>::iterator it = m_headers.begin(); it != m_headers.end(); it++)
    {
        m_outputHead += it->first + ": " + it->second + "\r\n";
    }
}