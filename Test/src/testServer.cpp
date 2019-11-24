#include <iostream>

#include "/home/staff/gach/zeroInstall/include/zmq.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>

int main()
{
    void *context = zmq_ctx_new();
    void *serverSocket = zmq_socket(context, ZMQ_PULL);
    int bindStatus = zmq_bind(serverSocket, "tcp://127.1.1.2:65298");
    if(bindStatus < 0)
    {
        std::cout << "Bind error: " << zmq_strerror(zmq_errno()) << std::endl;
        exit(1);
    }

    while(true)
    {
        unsigned char buffer[8192];
        int dataSize;
        dataSize = zmq_recv(serverSocket, buffer, 8192, 0);
        if(dataSize < 0)
        {
            std::cout << "Read error: " << zmq_strerror(zmq_errno()) << std::endl;
            exit(1);  
        }
        buffer[dataSize] = '\0';
        std::cout << "Received request: \n" << buffer << std::endl;

        char response[] = 
            "HTTP/1.0 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Hello, World!";

        std::cout << "Responding with: \n" << response << "\n################\n" << std::endl;
        dataSize = zmq_send(serverSocket, response, strlen(response), 0);
    }

    zmq_close(serverSocket);
    zmq_ctx_destroy(context);
    return 0;
}
