#ifndef __CONFIG__
#define __CONFIG__

#include <serverException.hpp>

#include <iostream>

#define LOGGER Config::getInstance().getLogger()

/**
 * @brief class Config stores configuration of server
 * 
 */
class Config 
{
public:
    /**
     * @brief Initializes server, could be called many times
     *  m_LoggerStream is std::cout by default
     * 
     * @param LoggerStream std::ostream & [INOUT] stream to which logs will be inserted
     */
    void init(std::ostream & LoggerStream)
    {
        m_LoggerStream = &LoggerStream;
    }

    /**
     * @brief Get the Instance object (singleton)
     * 
     * @return Config& Instance object
     */
    static Config & getInstance()
    {
        static Config instance;
        return instance;
    }

    /**
     * @brief Get the currently set Log object
     * 
     * @return std::ostream& 
     */
    std::ostream & getLogger()
    {
        SERVER_ASSERT_MSG(nullptr == m_LoggerStream, "Server not configured")
        return *m_LoggerStream;
    }
private:

    /**
     * @brief Construct a new Config object
     * 
     */
    Config() { m_LoggerStream = &std::cout; }

    std::ostream * m_LoggerStream;  //<? pointer to log stream instance
};

#endif /* __CONFIG__ */