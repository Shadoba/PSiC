#include <ProxyServer.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <zmq.h>
#include <iostream>
#include <sstream>

#include <DatagramHandler.hpp>
#include <Config.hpp>

#define BUFFER_SIZE 8192
#define ID_LENGTH 5


//
//SERVER SENDS AND RECEIVES ON THE SAME SOCKET, do zmq_connect(m_serverSocket) to send. A "connection", lasts for pretty much a single transfer, so clients will be reopening them constantly, be wary of those 0-frames
//


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
    for(int i = 0; i < m_connections.size(); i++)
        delete m_connections.at(i);
    zmq_close(m_serverSocket);
    zmq_ctx_destroy(m_context);
}

void ProxyServer::run()
{
    while(true)
    {
        //########################################################################
        //Up to 100 connections at a time - check on creation if vector is at 100?
        //########################################################################
        
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
        int status;
        std::stringstream dataStream;

        unsigned char id[ID_LENGTH];
        int idStatus;

        idStatus = zmq_recv(m_serverSocket, id, ID_LENGTH, ZMQ_DONTWAIT);
        if(idStatus < 0)
        {
            if(zmq_errno() == EAGAIN)
                continue;
            else
            {
                handleError(0);
            }
        }
        std::string idString = std::string((char*)id, ID_LENGTH);

        //THIS IMPLEMENTATION ASSUMES SERVER RESPONSES ARE RECEIVED BY THE SAME SOCKET AS CLIENT REQUESTS - serverSocket.

        unsigned int i = 0;
        ProxyConnection *currentConnection;
        for(; i < m_connections.size(); i++)
        {
            currentConnection = m_connections.at(i);
            if(!currentConnection->getClientId().compare(idString))                                  //Message from existing client
            {
                currentConnection->updateTimer();
                status = zmq_recv(m_serverSocket, buffer, BUFFER_SIZE, 0);
                if(status < 0)
                {
                    handleError(status);
                    break;
                }
                else if(status == 0)
                {
                    closeConnection(currentConnection->getServerId());
                    dropConnection(i);
                    break;
                }

                if(status > BUFFER_SIZE)
                {
                    respondWith413(idString);
                    break;
                }

                dataStream.write((char*)buffer, status);
                
                if(currentConnection->getSecure())
                {
                    sendMessage(currentConnection->getServerId(), dataStream.str());
                }
                else
                {
                    dataStream.put('\0');
                    DatagramHandler datagramHandler = DatagramHandler(dataStream.str());
                    sendMessage(currentConnection->getServerId(), datagramHandler.OutputDatagram);
                }
                
                break;
            }
            else if(!currentConnection->getServerId().compare(idString))                             //Message from server
            {
                currentConnection->updateTimer();
                do
                {
                    zmq_recv(m_serverSocket, id, ID_LENGTH, 0);                                     //Id frame
                    status = zmq_recv(m_serverSocket, buffer, BUFFER_SIZE, 0);
                    if(status < 0)
                        break;
                    if(status < BUFFER_SIZE)
                        dataStream.write((char*)buffer, status);
                    else
                        dataStream.write((char*)buffer, BUFFER_SIZE);
                } while(status > BUFFER_SIZE);

                if(status < 0)
                {
                    handleError(status);
                    break;
                }

                if(dataStream.str().empty())
                {
                    closeConnection(currentConnection->getClientId());
                    dropConnection(i);
                    break;
                }
                
                if(currentConnection->getSecure())
                {
                    sendMessage(currentConnection->getServerId(), dataStream.str());
                }
                else
                {
                    dataStream.put('\0');
                    DatagramHandler datagramHandler = DatagramHandler(dataStream.str());
                    sendMessage(currentConnection->getServerId(), datagramHandler.OutputDatagram);
                }

                break;
            }
        }

        if(i == m_connections.size())                                                           //Id not in m_connections means new connection from client
        {
            zmq_recv(m_serverSocket, buffer, 0, 0);                                             //To handle the empty "open connection" message
            //Handle what if this isn't empty??
            idStatus = zmq_recv(m_serverSocket, id, ID_LENGTH, 0);                              //Create a method bundling receive and error handling?
            if(idStatus < 0)
            {
                handleError(idStatus);
                break;
            }
            //compare IDs?

            status = zmq_recv(m_serverSocket, buffer, BUFFER_SIZE, 0);
            if(status < 0)
            {
                handleError(status);
            }
            else if(status > BUFFER_SIZE)
            {
                respondWith413(idString);
                closeConnection(idString);
            }
            else
            {
                dataStream.write((char*)buffer, BUFFER_SIZE);
                dataStream.put('\0');
                DatagramHandler datagramHandler = DatagramHandler(dataStream.str());
                if(datagramHandler.Protocol == protocol::protocol::INVALID)
                {
                    respondWith501(idString);
                    closeConnection(idString);
                }
                else
                {
                    std::string serverId = connectToServer(datagramHandler.Url);
                    if(!serverId.empty())
                    {
                        if(datagramHandler.RequestMethod != httpRequest::httpRequestMethod::CONNECT)
                        {
                            m_connections.push_back(ProxyConnection(idString, serverId, false));
                            sendMessage(serverId, dataStream.str());
                        }
                        else
                        {
                            m_connections.push_back(ProxyConnection(idString, serverId, true));
                        }
                    }
                    else
                    {
                        respondWith502(idString);                                                   //client ID
                        closeConnection(idString);
                    }
                }
            }
        }

        for(unsigned int i = 0; i < m_connections.size(); i++)
        {
            if(difftime(m_connections.at(i)->getTimer(), std::time()) > 60.0f)
            {
                closeConnection(m_connections.at(i)->getClientId());
                closeConnection(m_connections.at(i)->getServerId());
                dropConnection(i);
                i--;
            }
        }
    }
}

std::string ProxyServer::connectToServer(std::string url)
{
    std::string address;
    int status;
    addrinfo *result;
    status = getaddrinfo(NULL, url.c_str(), NULL, &result);
    if(status != 0)
        handleError(status);

    for(addrinfo *addressInfo = result; addressInfo != NULL; addressInfo = addressInfo->ai_next)
    {
        sockaddr_in *addrin = (sockaddr_in*)addressInfo->ai_addr;
        address = std::string("tcp://") + std::string(inet_ntoa(addrin->sin_addr));
        address.append("\0");
        std::cout << address << std::endl;
        status = zmq_connect(m_serverSocket, address.c_str());
        if(status == 0)
            break;
    }

    freeaddrinfo(result);
    if(status != 0)
        return std::string();

    unsigned char idBuffer[ID_LENGTH];
    size_t intSize = ID_LENGTH;
    status = zmq_getsockopt(m_serverSocket, ZMQ_ROUTING_ID, idBuffer, &intSize);
    if(status != 0)
        handleError(status);
    return std::string((char*)idBuffer, ID_LENGTH);
}

void ProxyServer::sendMessage(std::string id, std::string message)
{
    zmq_send(m_serverSocket, id.c_str(), id.size(), ZMQ_SNDMORE);
    zmq_send(m_serverSocket, message.c_str(), message.size(), 0);
}

void ProxyServer::closeConnection(std::string id)
{
    zmq_send(m_serverSocket, id.c_str(), ID_LENGTH, ZMQ_SNDMORE);
    zmq_send(m_serverSocket, 0, 0, 0);
}

void ProxyServer::dropConnection(unsigned int i)
{
    m_connections.erase(m_connections.begin() + i);
}

void ProxyServer::respondWith413(std::string id)
{
    std::string toSend = std::string("HTTP/1.1 413 Payload Too Large\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "\r\n"
                                     "Payload Too Large");
    sendMessage(id, toSend);
}

void ProxyServer::respondWith501(std::string id)
{
    std::string toSend = std::string("HTTP/1.1 501 Not Implemented\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "\r\n"
                                     "Not Implemented");
    sendMessage(id, toSend);
}

void ProxyServer::respondWith502(std::string id)
{
    std::string toSend = std::string("HTTP/1.1 502 Bad Gateway\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "\r\n"
                                     "Bad Gateway");
    sendMessage(id, toSend);
}

void ProxyServer::handleError(int status)
{
    LOGGER << "Error during name resolution/establishing connection: " << zmq_strerror(zmq_errno()) << std::endl;
}