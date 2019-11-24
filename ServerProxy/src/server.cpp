#include <iostream>

#include <zmq.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>
#include <cassert>
#include <sstream>

//https://linux.die.net/man/3/getaddrinfo FOR NAME RESOLUTION
//ZMQ_STREAM socket mode - works differently to normal!

class ProxyConnection
{
public:
    ProxyConnection(void *clientSocket, std::string clientId, void *serverSocket, std::string serverId, bool secure)
    {
        this->clientSocket = clientSocket;
        this->serverSocket = serverSocket;
        this->secure = secure;
    }

    void *getClientSocket()
    {
        return clientSocket;
    }

    std::string getClientId()
    {
        return clientId;
    }

    void *getServerSocket()
    {
        return serverSocket;
    }

    std::string getServerId()
    {
        return serverId;
    }

    bool getSecure()
    {
        return secure;
    }


protected:
    void *clientSocket;
    std::string clientId;
    void *serverSocket;
    std::string serverId;
    bool secure;
};

#define BUFFER_SIZE 8192
#define ID_LENGTH 5

int main()
{
    void *context = zmq_ctx_new();
    void *serverSocket = zmq_socket(context, ZMQ_STREAM);
    int bindStatus = zmq_bind(serverSocket, "tcp://127.1.1.1:65299");
    if(bindStatus < 0)
    {
        std::cout << "Bind error: " << zmq_strerror(zmq_errno()) << std::endl;
        exit(1);
    }

    while(true)
    {
        unsigned char id[256];
        int idSize;

        unsigned char data[BUFFER_SIZE];
        int dataSize;
        std::stringstream dataStream;

        idSize = zmq_recv(serverSocket, id, 256, 0);
        if(idSize <= 0)
        {
            std::cout << "Receive error: " << zmq_strerror(zmq_errno()) << std::endl;
            exit(1);
        }
        assert(idSize == ID_LENGTH);
        std::string idString((char*)id, ID_LENGTH);
        std::cout << "Received ID: " << idString << std::endl;

        dataSize = zmq_recv(serverSocket, data, 0, 0);                //Pusty recv, otwiera połączenie
        do {
            zmq_recv(serverSocket, id, ID_LENGTH, 0);          //Znowu ID, można porównać z tym wcześniej otrzymanym, powinno być unikatowe dla połączenia
            assert(!idString.compare(std::string((char*)id, ID_LENGTH)));
            dataSize = zmq_recv(serverSocket, data, BUFFER_SIZE, 0);
            std::cout << "Received partial data, size: " << dataSize << std::endl;
            dataStream.write((char*)data, dataSize);
        } while(dataSize == BUFFER_SIZE);
        if(dataSize < 0)
        {
            std::cout << "Receive error: " << zmq_strerror(zmq_errno()) << std::endl;
            exit(1);
        }
        dataStream << '\0';

        //Parse request, connect to server, create a ProxyConnection object and store it.
        //TODO

        std::cout << "Data received:\n" << dataStream.str() << "\n" << std::endl;

        char http_response [] =
            "HTTP/1.0 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Hello, World!";
        int sendSize = zmq_send(serverSocket, id, idSize, ZMQ_SNDMORE);
        if(sendSize < 0)
        {
            std::cout << "ID send error: " << zmq_strerror(zmq_errno()) << std::endl;
            exit(1);
        }
        std::cout << "Sent ID, bytes:" << sendSize << std::endl;

        sendSize = zmq_send(serverSocket, http_response, strlen(http_response), 0);
        if(sendSize < 0)
        {
            std::cout << "Data send error: " << zmq_strerror(zmq_errno()) << std::endl;
            exit(1);
        }
        std::cout << "Sent data, bytes:" << sendSize << std::endl;

        //Close connection
        zmq_send(serverSocket, id, idSize, ZMQ_SNDMORE);
        zmq_send(serverSocket, 0, 0, 0);
    }

    zmq_close(serverSocket);
    zmq_ctx_destroy(context);

    return 0;
}
