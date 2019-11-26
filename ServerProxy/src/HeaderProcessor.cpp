#include "HeaderProcessor.hpp"

#include "Utility.hpp"

#include <iostream>

HeaderProcessor::HeaderProcessor(std::string head) :
    InputHeader(m_head),
    Method(m_method),
    Url(m_url),
    Uri(m_uri),
    Protocol(m_protocol),
    OutputHeader(m_outputHeader)
{
    m_head = head;
    pullDataFromHead(head);
    if(m_method.compare(std::string("CONNECT")))        //0 to równe, czyli wchodzi jeśli nie "CONNECT"
        transformHead(head);
}

std::map<std::string, std::string> HeaderProcessor::getFields() const
{
    return m_fields;
}

void HeaderProcessor::pullDataFromHead(std::string head)
{
    std::vector<std::string> lines = Utility::splitString(head, "\r\n");
    std::vector<std::string> firstLineWords = Utility::splitString(lines.at(0), ' ');
    lines.erase(lines.begin());
    m_method = firstLineWords.at(0);
    m_url = firstLineWords.at(1);
    m_protocol = firstLineWords.at(2);

    calculateUri();

    for(unsigned int i = 0; i < lines.size(); i++)
    {
        if(!lines.at(i).empty())
        {
            std::vector<std::string> field = Utility::splitString(lines.at(i), ": ");
            m_fields.insert(std::pair<std::string, std::string>(field.at(0), field.at(1)));
        }
    }
}

//Na razie tylko przepisuje i podmienia URL, jeśli trzeba
void HeaderProcessor::transformHead(std::string head)
{
    m_outputHeader += m_method + " ";
    m_outputHeader += m_uri + " ";
    m_outputHeader += m_protocol + "\r\n";
    for(std::map<std::string, std::string>::iterator it = m_fields.begin(); it != m_fields.end(); it++)
    {
        m_outputHeader += it->first + ": " + it->second + "\r\n";
    }
}

void HeaderProcessor::calculateUri()
{
    size_t position = m_url.find('/');
    if(position != 0)                                       //Jeśli pierwszym znakiem nie jest '/', to mamy pełen URL
    {
        position = m_url.find('/', position + 2);
        if(position == std::string::npos)                   //Jeśli poza "http://" nie ma '/', to musimy dodać sam '/'
            m_uri = std::string("/");
        else
            m_uri = std::string(m_url.substr(position));
    }
    else
        m_uri = m_url;
}

httpRequestMethod HeaderProcessor::getHttpRequestMethodEnum()
{
    return Utility::httpRequestMethodMap[m_method];
}

protocol HeaderProcessor::getProtocolEnum()
{
    return Utility::protocolMap[m_protocol];
}