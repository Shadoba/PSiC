#include <ProxyConnection.hpp>

#include <zmq.h>

#include <Config.hpp>

ProxyConnection::ProxyConnection(unsigned char * clientId, unsigned char * serverId, bool secure) : 
    m_clientId(clientId, 5), m_serverId(serverId, 5), m_secure(secure)
{
    #if LOG_LEVEL == 1
        LOGGER << "New connection" << std::endl;
    #endif
}

ProxyConnection::~ProxyConnection()
{

}

const std::string & ProxyConnection::getClientId() const
{
    return m_clientId;
}

const std::string & ProxyConnection::getServerId() const
{
    return m_serverId;
}

const bool ProxyConnection::getSecure() const
{
    return m_secure;
}