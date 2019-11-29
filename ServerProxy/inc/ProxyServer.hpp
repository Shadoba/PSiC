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
    std::string openConnection(std::string url);
    void sendMessage(std::string id, std::string message);
    void closeConnection(std::string id);
    void respondWith419(std::string id);
    void respondWith502(std::string id);
    void handleError(int status);

    std::vector<ProxyConnection*> m_connections;
    void *m_context;
    void *m_serverSocket;
};
