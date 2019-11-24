#include <ProxyConnection.hpp>
#include <Config.hpp>

ProxyConnection::ProxyConnection(void *clientSocket, unsigned char * clientId, void *serverSocket, unsigned char * serverId, bool secure) : 
    m_clientSocket(clientSocket), m_clientId(clientId, 5), 
    m_serverSocket(serverSocket), m_serverId(serverId, 5),
    m_secure(secure)
{
    #if LOG_LEVEL == 1
        LOGGER << "New connection" << std::endl;
    #endif
}

const void * ProxyConnection::getClientSocket() const
{
    return m_clientSocket;
}

const std::basic_string<unsigned char> & ProxyConnection::getClientId() const
{
    return m_clientId;
}

const void * ProxyConnection::getServerSocket() const
{
    return m_serverSocket;
}

const std::basic_string<unsigned char> & ProxyConnection::getServerId() const
{
    return m_serverId;
}

const bool ProxyConnection::getSecure() const
{
    return m_secure;
}