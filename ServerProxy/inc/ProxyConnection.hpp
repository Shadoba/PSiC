#ifndef __PROXYCONNECTION__
#define __PROXYCONNECTION__

#include <string>


class ProxyConnection
{
public:
    ProxyConnection(unsigned char * clientId, unsigned char * serverId, bool secure);
    ~ProxyConnection();

    const std::string & getClientId() const;

    const std::string & getServerId() const;

    const bool getSecure() const;


protected:
    const std::string m_clientId;
    const std::string m_serverId;
    const bool m_secure;
};

#endif /* __PROXYCONNECTION__ */