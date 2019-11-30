#ifndef __PROXYCONNECTION__
#define __PROXYCONNECTION__

#include <string>

/**
 * @brief Class ProxyConnection stores data on 
 *  connection between two peers
 */
class ProxyConnection
{
public:
    ProxyConnection(unsigned char * clientId, unsigned char * serverId, bool secure);
    ~ProxyConnection();

    /**
     * @brief Get the Client Id object
     * 
     * @return const std::basic_string <unsigned char>& id of seed
     */
    const std::string & getClientId() const;

    /**
     * @brief Get the Server Id object
     * 
     * @return const std::basic_string <unsigned char>& id of peer
     */
    const std::string & getServerId() const;

    /**
     * @brief Get the Secure object
     * 
     * @return true connection is encrypted
     * @return false connection is not encrypted
     */
    const bool getSecure() const;

    const std::basic_string <unsigned char> m_clientId; //<? id of seed
    const std::basic_string <unsigned char> m_serverId; //<? id of peer
    const bool m_secure;                                //<? determines encryption
};

#endif /* __PROXYCONNECTION__ */