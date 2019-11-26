#pragma once

#include <string>
#include <map>

#include "Utility.hpp"

class HeaderProcessor
{
public:
    HeaderProcessor(const std::string head);

    const std::string & InputHeader;
    const std::string & Method;
    const std::string & Url;
    const std::string & Uri;
    const std::string & Protocol;
    const std::string & OutputHeader;
    std::map<std::string, std::string> getFields() const;

    httpRequestMethod getHttpRequestMethodEnum();
    protocol getProtocolEnum();

private:
    void pullDataFromHead(std::string head);
    void transformHead(std::string head);
    void calculateUri();

    std::string m_head;
    std::string m_method;
    std::string m_url;
    std::string m_uri;
    std::string m_protocol;
    std::string m_outputHeader;

    std::map<std::string, std::string> m_fields;
};
