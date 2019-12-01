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
    std::string connectToServer(std::string url);
    void sendMessage(std::string id, std::string message);
    void closeConnection(std::string id);
    void dropConnection(unsigned int i);
    void respondWith413(std::string id);
    void respondWith501(std::string id);
    void respondWith502(std::string id);
    void handleError(int status);

    std::vector<ProxyConnection*> m_connections;
    void *m_context;
    void *m_serverSocket;
};
