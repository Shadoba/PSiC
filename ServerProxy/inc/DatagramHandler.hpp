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

    const std::string & InputDatagram;      //<? Input datagram handle
    const std::string & OutputDatagram;     //<? Output datagram handle
    const httpRequestMethod & RequestMethod;//<? Request method handle
    const protocol & Protocol;              //<? Protocol handle

protected:
    const std::string m_InputDatagram;      //<? Input datagram
    std::string m_OutputDatagram;           //<? Output datagram
    httpRequestMethod m_RequestMethod;      //<? Request method
    protocol m_Protocol;                    //<? Protocol
};

#endif /* __DATAGRAMHANDLER__ */