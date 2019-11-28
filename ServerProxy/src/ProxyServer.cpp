#include <ProxyServer.hpp>

#include <zmq.h>
#include <iostream>
#include <sstream>

#define BUFFER_SIZE 8192
#define ID_LENGTH 5

ProxyServer::ProxyServer(const std::string str)
{
    m_context = zmq_ctx_new();
    m_serverSocket = zmq_socket(m_context, ZMQ_STREAM);
    int bindStatus = zmq_bind(m_serverSocket, str.c_str());
    if(bindStatus < 0)
    {
        std::cout << "Bind error: " << zmq_strerror(zmq_errno()) << std::endl;
        exit(1);
    }
}

ProxyServer::~ProxyServer()
{
    for(int i = 0; i < connections.size(); i++)
        delete connections.at(i);
    zmq_close(m_serverSocket);
    zmq_ctx_destroy(m_context);
}

void ProxyServer::run()
{
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
        //  If the ProxyConnection is marked secure:
        //  --No:
        //    look into the request body and use that number algorithm we are required to implement
        //  Send message to the other party

        unsigned char buffer[BUFFER_SIZE];
        int receiveStatus;
        std::stringstream message;

        unsigned char id[ID_LENGTH];
        int idStatus;

        idStatus = zmq_recv(m_serverSocket, id, ID_LENGTH, ZMQ_DONTWAIT);
        if(idStatus < 0)
        {
            if(zmq_errno() == EAGAIN)
                continue;
            else
            {
                std::cout << zmq_strerror(zmq_errno()) << std::endl;
                exit(1);
            }
        }
        std::string idString = std::string((char*)id, ID_LENGTH);

        //THIS IMPLEMENTATION ASSUMES SERVER RESPONSES ARE RECEIVED BY THE SAME SOCKET AS CLIENT REQUESTS - serverSocket.

        //PROBLEM: How can you map incoming messages to their recipents, if one client can be connected to many servers, to the same one many times, and one server to many clients
        //The entire thing below needs a rewrite because I didn't think of ^

        for(unsigned int i = 0; i < connections.size(); i++)
        {
            if(!connections.at(i).getClientId().compare(idString))                                  //Message from existing client
            {
                receiveStatus = zmq_recv(m_serverSocket, buffer, BUFFER_SIZE, 0);
                if(receiveStatus < 0)
                {
                    std::cout << zmq_strerror(zmq_errno()) << std::endl;
                    exit(1);
                }

                if(receiveStatus > BUFFER_SIZE)
                {
                    //respondWith413(idString);
                    //close connection? (send a 0 length message on this id)
                    break;
                }

                dataStream.write((char*)buffer, receiveStatus);
                
                if(connections.at(i).getSecure())
                {
                    //sendMessageToPeer(connections.at(i).getServerId(), dataStream.str());         //Do not involve DatagramHandler since the message is encrypted anyway
                }
                else
                {
                    dataStream.write("\0");
                    //modifyBody(datagramHandler);                                                  //This could be a method of DatagramHandler, so datagramHandler.modifyBody(), or it can do it  on its own
                    //sendMessageToPeer(connections.at(i).getServerId(), datagramHandler);
                }
                
                break;
            }
            else if(!connections.at(i).getServerId().compare(idString))                             //Message from server
            {
                do
                {
                    zmq_recv(m_serverSocket, id, ID_LENGTH, 0);                                       //Id frame
                    receiveStatus = zmq_recv(m_serverSocket, buffer, BUFFER_SIZE, 0);
                    if(receiveStatus < 0)
                    {
                        std::cout << zmq_strerror(zmq_errno()) << std::endl;
                        exit(1);
                    }
                    if(receiveStatus < BUFFER_SIZE)
                        dataStream.write((char*)buffer, receiveStatus);
                    else
                        dataStream.write((char*)buffer, BUFFER_SIZE);
                } while(receiveStatus > BUFFER_SIZE);

                //sendMessageToPeer(connections.at(i).getClientId());                               //since we're only checking the body of clinet requests, this one can just be sent

                break;
            }
            else                                                                                    //Since the ID isn't in connections, it's a new connection from a client
            {
                zmq_recv(m_serverSocket, buffer, 0, 0);                                               //To handle the empty "open connection" message
                idStatus = zmq_recv(m_serverSocket, id, ID_LENGTH, 0);                                //Create a method bundling receive and error handling?
                if(idStatus < 0)
                {
                    std::cout << zmq_strerror(zmq_errno()) << std::endl;
                    exit(1);
                }
                //compare IDs?

                receiveStatus = zmq_recv(m_serverSocket, buffer, BUFFER_SIZE, 0);
                if(receiveStatus < 0)
                {
                    std::cout << zmq_strerror(zmq_errno()) << std::endl;
                    exit(1);
                }

                if(receiveStatus > BUFFER_SIZE)
                {
                    //respondWith413(idString);
                    //close connection? (send a 0 length message on this id)
                    break;
                }
                else
                {
                    message.write((char*)buffer, BUFFER_SIZE);
                    message.write("\0");
                    //DatagramHandler datagramHandler = DatagramHandler(message.str());
                    //Extract server url, do name resolution, do zmq_connect
                    //Create a ProxyConnection and add it to the vector
                    //if it's not a CONNECT modify body and send to server
                }
                
                break;
            }
        }

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
}