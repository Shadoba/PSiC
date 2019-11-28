#pragma once

#include <vector>
#include <string>

#include <ProxyConnection.hpp>

class ProxyServer
{
public:
    ProxyServer(const std::string str);
    ~ProxyServer();

    void run();

private:
    std::vector<ProxyConnection*> m_connections;
    void *m_context;
    void *m_serverSocket;
};
