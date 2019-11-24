#include <iostream>

#include <zmq.h>
#include <Config.hpp>
#include <serverException.hpp>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>
#include <cassert>
#include <sstream>
#include <vector>

//https://linux.die.net/man/3/getaddrinfo FOR NAME RESOLUTION


#define BUFFER_SIZE 8192
#define ID_LENGTH 5

void run();

int main()
{
    Config::getInstance().init(std::cout);
    try
    {
        run();
    }
    catch(serverException & exception)
    {
        std::cout<<exception.what()<<std::endl;
    }
    return 0;
}

void run()
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
        std::string dataString;

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
        zmq_recv(serverSocket, id, ID_LENGTH, 0);                     //Znowu ID, można porównać z tym wcześniej otrzymanym, powinno być unikatowe dla połączenia
        assert(!idString.compare(std::string((char*)id, ID_LENGTH)));
        dataSize = zmq_recv(serverSocket, data, BUFFER_SIZE, 0);
        std::cout << "Received data, size: " << dataSize << std::endl;
        if(dataSize < 0)
        {
            std::cout << "Receive error: " << zmq_strerror(zmq_errno()) << std::endl;
            exit(1);
        }
        if(dataSize > BUFFER_SIZE)
        {
            //Respond with 419 Payload Too Large
        }
        else
        {
            dataString = std::string(reinterpret_cast<char *>(data), dataSize);
            dataString.append("");

            //TODO: Parse request, connect to server, create a ProxyConnection object and store it.
        }

        //Close connection
        zmq_send(serverSocket, id, idSize, ZMQ_SNDMORE);
        zmq_send(serverSocket, 0, 0, 0);
    }

    zmq_close(serverSocket);
    zmq_ctx_destroy(context);
}