#include <ProxyConnection.hpp>

#include <zmq.h>

#include <Config.hpp>

ProxyConnection::ProxyConnection(std::string clientId, std::string serverId, bool secure) : 
    m_clientId(clientId), m_serverId(serverId), m_secure(secure)
{
    m_timer = std::time(NULL);

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

const time_t ProxyConnection::getTimer() const
{
    return m_timer;
}

void ProxyConnection::setTime(const time_t newTime)
{
    m_timer = newTime;
}

void ProxyConnection::updateTimer()
{
    m_timer = std::time(NULL);
}