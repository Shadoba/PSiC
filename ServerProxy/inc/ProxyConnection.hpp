#ifndef __PROXYCONNECTION__
#define __PROXYCONNECTION__

#include <array>


class ProxyConnection
{
public:
    ProxyConnection(void *clientSocket, std::array clientId, void *serverSocket, std::array serverId, bool secure) : 
        m_clientSocket(clientSocket), m_clientId(clientId), 
        m_serverSocket(serverSocket), m_serverId(serverId), 
        m_secure(secure)
    {}

    const void * getClientSocket() const
    {
        return clientSocket;
    }

    const std::string getClientId() const
    {
        return clientId;
    }

    const void * getServerSocket() const
    {
        return serverSocket;
    }

    const std::string getServerId() const
    {
        return serverId;
    }

    const bool getSecure() const
    {
        return secure;
    }


protected:
    const void * m_clientSocket;
    const std::array m_clientId;
    const void * m_serverSocket;
    const std::array m_serverId;
    const bool m_secure;
};

#endif /* __PROXYCONNECTION__ */