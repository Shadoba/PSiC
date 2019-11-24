#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string>

#include "/home/staff/gach/zeroInstall/include/zmq.h"

int main()
{
    void *context = zmq_ctx_new();
    void *clientSocket = zmq_socket(context, ZMQ_REQ);

    int connectStatus = zmq_connect(clientSocket, "tcp://127.1.1.1:65299");
    if(connectStatus < 0)
    {
        std::cout << "Error connecting: " << zmq_strerror(zmq_errno()) << std::endl;
        exit(1);
    }

    char toSend[] = "TEST";

    int sentSize = zmq_send(clientSocket, toSend, 4, 0);
    if(sentSize < 0)
    {
        std::cout << "Error sending: " << zmq_strerror(zmq_errno()) << std::endl;
        exit(1);
    }

    std::cout << "Sent message\n" << std::endl;

    char buffer[1024];
    int receiveSize = zmq_recv(clientSocket, buffer, 2013, 0);
    if(receiveSize < 0)
    {
        std::cout << "Error sending: " << zmq_strerror(zmq_errno()) << std::endl;
        exit(1);
    }
    buffer[receiveSize] = '\0';

    std::cout << "Received response:\n" << buffer << std::endl;

    zmq_close(clientSocket);
    zmq_ctx_destroy(context);
    return 0;
}
