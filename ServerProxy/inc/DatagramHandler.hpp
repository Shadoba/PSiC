#ifndef __DATAGRAMHANDLER__
#define __DATAGRAMHANDLER__

#include <string>

class DatagramHandler
{
public:
    explicit DatagramHandler(const std::string & datagram);

    //const std::string getInputDatagram() const;
    //const std::string getOutputDatagram() const;
    const std::string & InputDatagram;
    const std::string & OutputDatagram;
protected:
    const std::string m_InputDatagram;
    std::string m_OutputDatagram;


};

#endif /* __DATAGRAMHANDLER__ */