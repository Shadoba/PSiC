#pragma once

#include <serverException.hpp>
#include <Utility.hpp>

#include <string>
#include <map>

/**
 * @brief Class HeaderProcessor process' and stores header data
 * 
 */
class HeaderProcessor
{
public:
    /**
     * @brief Construct a new Header Processor object
     * 
     * @param head extracted from datagram header
     */
    HeaderProcessor(const std::string head);

    const std::string & InputHeader;    //<? input header handle
    const std::string & Method;         //<? method handle
    const std::string & Url;            //<? url handle
    const std::string & Uri;            //<? uri handle
    const std::string & Protocol;       //<? protocol handle
    const std::string & OutputHeader;   //<? output header handle
    /**
     * @brief Get the Fields object
     * 
     * @return std::map<std::string, std::string> map with all attributes from header
     */
    std::map<std::string, std::string> getFields() const;

    /**
     * @brief Get the Http Request Method Enum object
     * 
     * @return httpRequest::httpRequestMethod Http Request Method Enum
     */
    httpRequest::httpRequestMethod getHttpRequestMethodEnum();

    /**
     * @brief Get the Protocol Enum object
     * 
     * @return protocol::protocol Protocol Enum
     */
    protocol::protocol getProtocolEnum();

private:
    /**
     * @brief extract data from provided header and
     *  fills them the m_fields
     * @param head provided header
     */
    void pullDataFromHead(std::string head);

    /**
     * @brief tarnsforms output header to be ready to bee sent
     * 
     * @param head //TODO remove, kind a legacy argument 
     */
    void transformHead(std::string head);

    /**
     * @brief Extracts uri from url
     * 
     */
    void calculateUri();

    std::string m_head;         //<? input header
    std::string m_method;       //<? method
    std::string m_url;          //<? url
    std::string m_uri;          //<? uri
    std::string m_protocol;     //<? protocol
    std::string m_outputHeader; //<? output header
    std::map<std::string, std::string> m_fields;    //<? Attributes of header
};
