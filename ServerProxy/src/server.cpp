#include <zmq.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <Config.hpp>
#include <serverException.hpp>
#include <ProxyConnection.hpp>
#include <ProxyServer.hpp>

//https://linux.die.net/man/3/getaddrinfo FOR NAME RESOLUTION

int main()
{
    Config::getInstance().init(std::cout);
    ProxyServer server = ProxyServer(std::string("tcp://127.1.1.1:65299"));
    try
    {
        server.run();
    }
    catch(serverException & exception)
    {
        std::cout << exception.what() << std::endl;
    }
    return 0;
}
