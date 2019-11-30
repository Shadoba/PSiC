#ifndef __DATAGRAMHANDLER__
#define __DATAGRAMHANDLER__

#include <string>
#include <Utility.hpp>

/**
 * @brief Class DatagramHandler holds input and 
 *  output datagram, handles processing
 */
class DatagramHandler
{
public:
    /**
     * @brief Construct a new datagram handler object
     * 
     * @param datagram std::string [IN] input datagram
     */
    explicit DatagramHandler(const std::string & datagram);

    const std::string & InputDatagram;                  //<? Input datagram handle
    const std::string & OutputDatagram;                 //<? Output datagram handle
    const httpRequest::httpRequestMethod & RequestMethod;//<? Request method handle
    const protocol::protocol & Protocol;                //<? Protocol handle
    const std::string & Url;                            //<? url handle

protected:
    const std::string m_InputDatagram;              //<? Input datagram
    std::string m_OutputDatagram;                   //<? Output datagram
    const httpRequest::httpRequestMethod m_RequestMethod; //<? Request method
    const protocol::protocol m_Protocol;                  //<? Protocol
    const std::string m_url                               //<? url handle

private:
    static int processDatagramBody(std::string & body);
    static int processParsePrepare(std::string & body);
};

#endif /* __DATAGRAMHANDLER__ */