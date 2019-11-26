#ifndef __CONFIG__
#define __CONFIG__

#include <serverException.hpp>

#include <iostream>

#define LOGGER Config::getInstance().getLogger()

class Config 
{
public:
    void init(std::ostream & LoggerStream)
    {
        m_LoggerStream = &LoggerStream;
    }
    static Config & getInstance()
    {
        static Config instance;
        return instance;
    }
    std::ostream & getLogger()
    {
        SERVER_ASSERT_MSG(nullptr == m_LoggerStream, "Server not configured")
        return *m_LoggerStream;
    }
private:
    Config() { m_LoggerStream = nullptr; }
    std::ostream * m_LoggerStream;
};

#endif /* __CONFIG__ */