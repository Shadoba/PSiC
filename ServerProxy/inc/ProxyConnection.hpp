#ifndef __PROXYCONNECTION__
#define __PROXYCONNECTION__

#include <string>
#include <time.h>
#include <ctime>

/**
 * @brief Class ProxyConnection stores data on 
 *  connection between two peers
 */
class ProxyConnection
{
public:
    ProxyConnection(std::string clientId, std::string serverId, bool secure);
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

    /**
     * @brief Get the Timer object
     * 
     * @return time_t of timer
     */
    const time_t getTimer() const;

    /**
     * @brief Sets the Timer Object
     * 
     * @param newTime time_t new value of Timer
     */
    void setTime(const time_t newTime);

    /**
     * @brief Updates the Timer object to current moment
     */
    void updateTimer();

private:
    const std::string m_clientId; //<? id of seed
    const std::string m_serverId; //<? id of peer
    const bool m_secure;          //<? determines encryption
    time_t m_timer;
};

#endif /* __PROXYCONNECTION__ */