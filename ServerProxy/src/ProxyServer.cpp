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
        LOGGER << "Bind error: " << zmq_strerror(zmq_errno()) << std::endl;
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
        for(unsigned int i = 0; i < m_connections.size(); i++)
        {
            if(difftime(m_connections.at(i)->getTimer(), std::time(NULL)) > 60.0f)
            {
                closeConnection(m_connections.at(i)->getClientId());
                closeConnection(m_connections.at(i)->getServerId());
                dropConnection(i);
                i--;
            }
        }
        
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

        idStatus = zmq_recv(m_serverSocket, id, ID_LENGTH, 0);
        if(idStatus < 0)
        {
            if(zmq_errno() == EAGAIN)
                continue;
            else
            {
                handleError(idStatus);
            }
        }
        std::string idString = std::string((char*)id, ID_LENGTH);
        #if LOG_LEVEL > 5
            LOGGER << "Received message at " << idString << std::endl;
        #endif

        unsigned int i = 0;
        ProxyConnection *currentConnection;
        for(; i < m_connections.size(); i++)
        {
            currentConnection = m_connections.at(i);
            if(!currentConnection->getClientId().compare(idString))                                  //Message from existing client
            {
                #if LOG_LEVEL > 5
                    LOGGER << "Received message from client " << currentConnection->getClientId() << std::endl;
                #endif
                currentConnection->updateTimer();
                status = zmq_recv(m_serverSocket, buffer, BUFFER_SIZE, 0);
                if(status < 0)
                {
                    handleError(status);
                }
                else if(status == 0)
                {
                    #if LOG_LEVEL > 5
                        LOGGER << "Connection closed from " << idString << std::endl;
                    #endif
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
                dataStream.put('\0');
                std::string data = dataStream.str();

                #if LOG_LEVEL > 5
                    LOGGER << "Data size " << status << ", content\n" << data << std::endl;
                #endif

                if(currentConnection->getSecure())
                {
                    sendMessage(currentConnection->getServerId(), data);
                }
                else
                {
                    DatagramHandler datagramHandler = DatagramHandler(data);
                    sendMessage(currentConnection->getServerId(), datagramHandler.OutputDatagram);
                }
                
                break;
            }
            else if(!currentConnection->getServerId().compare(idString))                             //Message from server
            {
                #if LOG_LEVEL > 5
                    LOGGER << "Received message from server " << currentConnection->getServerId() << std::endl;
                #endif
                currentConnection->updateTimer();
                do
                {
                    zmq_recv(m_serverSocket, id, ID_LENGTH, 0);                                      //Id frame
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
                }

                if(dataStream.str().empty())
                {
                    #if LOG_LEVEL > 5
                        LOGGER << "Closed connection from " << idString << std::endl;
                    #endif
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
            #if LOG_LEVEL > 5
                LOGGER << "Received new connection " << idString << std::endl;
            #endif
            status = zmq_recv(m_serverSocket, buffer, BUFFER_SIZE, 0);                                             //To handle the empty "open connection" message
            if(status != 0)
            {
                LOGGER << "Received data where there should be none!" << std::endl;
                exit(1);
            }
            //Handle what if this isn't empty??
            idStatus = zmq_recv(m_serverSocket, id, ID_LENGTH, 0);                              //Create a method bundling receive and error handling?
            if(idStatus < 0)
            {
                handleError(idStatus);
            }
            std::string newIdString = std::string((char*)id);
            if(!newIdString.compare(idString))
            {
                LOGGER << "Id mismatch! " << idString << " and " << newIdString << std::endl;
                exit(1);
            }

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
                dataStream.write((char*)buffer, status);
                dataStream.put('\0');
                DatagramHandler datagramHandler = DatagramHandler(dataStream.str());
                if(datagramHandler.Protocol == protocol::protocol::INVALID)
                {
                    respondWith501(idString);
                    closeConnection(idString);
                }
                else
                {
                    #if LOG_LEVEL > 5
                        LOGGER << "Attempting to connect to server at " << datagramHandler.Url << std::endl;
                    #endif
                    std::string serverId = connectToServer(datagramHandler.Url);
                    if(!serverId.empty())
                    {
                        #if LOG_LEVEL > 5
                            LOGGER << "Connection established between " << idString << " and " << serverId << std::endl;
                        #endif
                        if(datagramHandler.RequestMethod != httpRequest::httpRequestMethod::CONNECT)
                        {
                            #if LOG_LEVEL > 5
                                LOGGER << "Request method insecure, sending to server" << std::endl;
                            #endif
                            m_connections.push_back(new ProxyConnection(idString, serverId, false));
                            sendMessage(serverId, dataStream.str());
                        }
                        else
                        {
                            #if LOG_LEVEL > 5
                                LOGGER << "Request method secure, sending to server" << std::endl;
                            #endif
                            m_connections.push_back(new ProxyConnection(idString, serverId, true));
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

    }
}

std::string ProxyServer::connectToServer(std::string url)
{
    std::string address;
    int status;
    addrinfo *result;
    addrinfo hints;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = 0;
    std::string extractedUrl = Utility::extractDomainName(url);
    #if LOG_LEVEL > 5
        LOGGER << "Extracted URL " << extractedUrl << std::endl;
    #endif
    status = getaddrinfo(extractedUrl.c_str(), NULL, &hints, &result);
    if(status < 0)
        handleError(status);

    for(addrinfo *addressInfo = result; addressInfo != NULL; addressInfo = addressInfo->ai_next)
    {
        sockaddr_in *addrin = (sockaddr_in*)addressInfo->ai_addr;
        address = std::string("tcp://") + std::string(inet_ntoa(addrin->sin_addr)) + std::string(":8080");
        #if LOG_LEVEL > 5
            LOGGER << "Resolved server address " << address << std::endl;
        #endif
        status = zmq_connect(m_serverSocket, address.c_str());
        if(status == 0)
            break;
    }

    freeaddrinfo(result);
    if(status < 0)
        return std::string();

    #if LOG_LEVEL > 5
        LOGGER << "Used server address " << address << std::endl;
    #endif
    unsigned char idBuffer[ID_LENGTH];
    size_t intSize = ID_LENGTH;
    status = zmq_getsockopt(m_serverSocket, ZMQ_ROUTING_ID, idBuffer, &intSize);
    if(status < 0)
        handleError(status);
    return std::string((char*)idBuffer, ID_LENGTH);
}

void ProxyServer::sendMessage(std::string id, std::string message)
{
    int status;
    status = zmq_send(m_serverSocket, id.c_str(), id.size(), ZMQ_SNDMORE);
    if(status < 0)
        handleError(status);
    status = zmq_send(m_serverSocket, message.c_str(), message.size(), 0);
    if(status < 0)
        handleError(status);
}

void ProxyServer::closeConnection(std::string id)
{
    #if LOG_LEVEL > 5
        LOGGER << "Closing connection to " << id << std::endl;
    #endif
    zmq_send(m_serverSocket, id.c_str(), ID_LENGTH, ZMQ_SNDMORE);
    zmq_send(m_serverSocket, 0, 0, 0);
}

void ProxyServer::dropConnection(unsigned int i)
{
    #if LOG_LEVEL > 5
        LOGGER << "Dropping connection from " << m_connections.at(i)->getClientId() << " to " << m_connections.at(i)->getServerId() << std::endl;
    #endif
    m_connections.erase(m_connections.begin() + i);
}

//Those responses could be handled by a map?
void ProxyServer::respondWith413(std::string id)
{
    #if LOG_LEVEL > 5
        LOGGER << "Payload too large!" << std::endl;
    #endif
    std::string toSend = std::string("HTTP/1.1 413 Payload Too Large\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "\r\n"
                                     "Payload Too Large");
    sendMessage(id, toSend);
}

void ProxyServer::respondWith501(std::string id)
{
    #if LOG_LEVEL > 5
        LOGGER << "Invalid protocol!" << std::endl;
    #endif
    std::string toSend = std::string("HTTP/1.1 501 Not Implemented\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "\r\n"
                                     "Not Implemented");
    sendMessage(id, toSend);
}

void ProxyServer::respondWith502(std::string id)
{
    #if LOG_LEVEL > 5
        LOGGER << "Unable to connect to server!" << std::endl;
    #endif
    std::string toSend = std::string("HTTP/1.1 502 Bad Gateway\r\n"
                                     "Content-Type: text/plain\r\n"
                                     "\r\n"
                                     "Bad Gateway");
    sendMessage(id, toSend);
}

void ProxyServer::handleError(int status)
{
    if(zmq_errno() != 0)
        LOGGER << "Error: " << zmq_strerror(zmq_errno()) << std::endl;
    else
        LOGGER << "Error: " << gai_strerror(errno) << std::endl;
    exit(1);
}
