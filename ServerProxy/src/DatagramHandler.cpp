#include <DatagramHandler.hpp>
#include <HeaderProcessor.hpp>
#include <Utility.hpp>

DatagramHandler::DatagramHandler(const std::string & datagram) :
    InputDatagram(m_InputDatagram), 
    OutputDatagram(m_OutputDatagram),
    m_InputDatagram(datagram)
{
    const char * const headerEnd = "\r\n\r\n";
    std::vector<std::string> splitDatagram = Utility::splitString(m_InputDatagram, headerEnd);
    std::string & header = splitDatagram[0];
    const std::string & body = splitDatagram[1];

    HeaderProcessor headerProcessor(header);

    m_OutputDatagram = headerProcessor.OutputHeader;
    m_OutputDatagram += headerEnd;
    m_OutputDatagram += body;
};