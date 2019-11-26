#include <iostream>

#include <zmq.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>
#include <cassert>
#include <sstream>
#include <vector>

#include <Config.hpp>
#include <serverException.hpp>
#include <ProxyConnection.hpp>


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

    std::vector<ProxyConnection> connections;
    while(true)
    {
        //Up to 100 connections at a time - check on creation if vector is at 100?
        //Respond with 413 if the request from client is too large - above 8192 bytes
        //Close connections inactive for over 1 minute

        //Recv NOBLOCK, skip the entire thing if no message available, increase an inactivity counter?
        //Create DatagramHandler           //NEED TO CHECK IF THE MESSAGE IS A REQUEST OR RESPONSE!
                                           //Based on where it came from? Then this has to be created later, after the connections check
                                           //Based on the first line? Check if the third word is a number (status code). Can be done here.
        //Discard and respond with error if invalid method/protocol
        //If already in connections
        //--No: (this means it's a request from the client)
        //  resolve target IP
        //  zmq_connect to target (if error respond to client with 502)
        //  get ID frame
        //  create ProxyConnection and add to vector
        //  if appropriate, send data to server (do not send if it's CONNECT, just set up the ProxyConnection correctly)
        //--Yes:
        //  If the Proxyconnection is marked secure:
        //  --No:
        //    look into the request body and use that number algorithm we are required to implement
        //  Send message to the other party

        /*
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
            dataString.append("\0");

            //TODO: Parse request, connect to server, create a ProxyConnection object and store it.
        }

        //Close connection
        zmq_send(serverSocket, id, idSize, ZMQ_SNDMORE);
        zmq_send(serverSocket, 0, 0, 0);
        */
    }

    zmq_close(serverSocket);
    zmq_ctx_destroy(context);
}