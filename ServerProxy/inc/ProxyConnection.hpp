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
    /**
     * @brief Construct a new Proxy Connection object
     * 
     * @param clientSocket void* [IN] pointer to seed socket
     * @param clientId unsigned char * [IN] id of seed
     * @param serverSocket void* [IN] pointer to peer socket
     * @param serverId unsigned char * [IN] id of peer
     * @param secure bool [IN] true if connection is encrypted
     */
    ProxyConnection(void *clientSocket, unsigned char * clientId, void *serverSocket, unsigned char * serverId, bool secure);

    /**
     * @brief Get the Client Socket object
     * 
     * @return const void* pointer to seed socket
     */
    const void * getClientSocket() const;

    /**
     * @brief Get the Client Id object
     * 
     * @return const std::basic_string <unsigned char>& id of seed
     */
    const std::basic_string <unsigned char> & getClientId() const;

    /**
     * @brief Get the Server Socket object
     * 
     * @return const void* peer socket
     */
    const void * getServerSocket() const;

    /**
     * @brief Get the Server Id object
     * 
     * @return const std::basic_string <unsigned char>& id of peer
     */
    const std::basic_string <unsigned char> & getServerId() const;

    /**
     * @brief Get the Secure object
     * 
     * @return true connection is encrypted
     * @return false connection is not encrypted
     */
    const bool getSecure() const;


protected:
    const void * m_clientSocket;                        //<? pointer to seed socket
    const std::basic_string <unsigned char> m_clientId; //<? id of seed
    const void * m_serverSocket;                        //<? pointer to peer socket
    const std::basic_string <unsigned char> m_serverId; //<? id of peer
    const bool m_secure;                                //<? determines encryption
};

#endif /* __PROXYCONNECTION__ */