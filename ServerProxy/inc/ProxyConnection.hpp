#ifndef __PROXYCONNECTION__
#define __PROXYCONNECTION__

#include <string>


class ProxyConnection
{
public:
    ProxyConnection(void *clientSocket, unsigned char * clientId, void *serverSocket, unsigned char * serverId, bool secure);

    const void * getClientSocket() const;

    const std::basic_string <unsigned char> & getClientId() const;

    const void * getServerSocket() const;

    const std::basic_string <unsigned char> & getServerId() const;

    const bool getSecure() const;


protected:
    const void * m_clientSocket;
    const std::basic_string <unsigned char> m_clientId;
    const void * m_serverSocket;
    const std::basic_string <unsigned char> m_serverId;
    const bool m_secure;
};

#endif /* __PROXYCONNECTION__ */