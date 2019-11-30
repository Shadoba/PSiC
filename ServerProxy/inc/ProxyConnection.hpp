#ifndef __PROXYCONNECTION__
#define __PROXYCONNECTION__

#include <string>


class ProxyConnection
{
public:
    ProxyConnection(unsigned char * clientId, unsigned char * serverId, bool secure);
    ~ProxyConnection();

    const std::basic_string <unsigned char> & getClientId() const;

    const std::basic_string <unsigned char> & getServerId() const;

    const bool getSecure() const;


protected:
    const std::basic_string <unsigned char> m_clientId;
    const std::basic_string <unsigned char> m_serverId;
    const bool m_secure;
};

#endif /* __PROXYCONNECTION__ */