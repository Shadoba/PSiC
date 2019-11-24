#ifndef __CONFIG__
#define __CONFIG__

#include <iostream>

#define LOGGER Config::getInstance().Logger

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
    const std::ostream & Logger;
private:
    Config() : Logger(*m_LoggerStream){}
    std::ostream * m_LoggerStream;
};

#endif /* __CONFIG__ */