#include <DatagramHandler.hpp>
#include <HeaderProcessor.hpp>
#include <Config.hpp>

DatagramHandler::DatagramHandler(const std::string & datagram) :
    InputDatagram(m_InputDatagram), 
    OutputDatagram(m_OutputDatagram),
    RequestMethod(m_RequestMethod),
    Protocol(m_Protocol),
    m_InputDatagram(datagram)
{
    /* Splits datagram to header and    *
     * body. Lets the HeaderProcessor   *
     * process the header               */
    const char * const headerEnd = "\r\n\r\n";
    std::vector<std::string> splitDatagram = Utility::splitStringOnce(m_InputDatagram, headerEnd);
    #if LOG_LEVEL > 5
    LOGGER << "Header split into: " << splitDatagram.size() << std::endl;
    #endif
    SERVER_ASSERT_MSG(2 != splitDatagram.size(), "splitStringOnce() more than one split")

    std::string & header = splitDatagram[0];
    const std::string & body = splitDatagram[1];

    HeaderProcessor headerProcessor(header);

    /* Reassemble output datagram from  *
     * headerProcessor processed data   *
     * and assign protol and method enum*/
    m_OutputDatagram = headerProcessor.OutputHeader;
    m_OutputDatagram += headerEnd;
    m_OutputDatagram += body;

    m_RequestMethod = headerProcessor.getHttpRequestMethodEnum();
    m_Protocol = headerProcessor.getProtocolEnum();
};