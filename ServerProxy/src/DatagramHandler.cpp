#include <DatagramHandler.hpp>
#include <HeaderProcessor.hpp>
#include <Config.hpp>
#include <serverException.hpp>

DatagramHandler::DatagramHandler(const std::string & datagram) :
    InputDatagram(m_InputDatagram), 
    OutputDatagram(m_OutputDatagram),
    RequestMethod(m_RequestMethod),
    Protocol(m_Protocol),
    Url(m_url),
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
    m_RequestMethod = headerProcessor.getHttpRequestMethodEnum();
    m_Protocol = headerProcessor.getProtocolEnum();
    m_url = headerProcessor.Url;

    if(httpRequest::CONNECT != m_RequestMethod)
        int result = processDatagramBody(body);

    m_OutputDatagram = headerProcessor.OutputHeader;
    m_OutputDatagram += headerEnd;
    m_OutputDatagram += body;

};

// int DatagramHandler::processDatagramBody(std::string & body)
// {
//     __int128 numberStorage;

//     std::string bodyTmp = body;
//     int numPos = 0;
//     int numPosAbsoulte = 0;
//     //bodyTmp.erase(std::remove(bodyTmp.begin(), bodyTmp.end(), ' '), bodyTmp.end());
//     while(numPos = bodyTmp.find_first_of("0123456789"))
//     {
//         int counter = 0;
//         do
//         {
//             counter++;
//         } while (validateNumber);

//         if(26 <= counter)
//         {
//             break;
//         }
//         else
//         {
//             /* code */
//         }
//     }
// }