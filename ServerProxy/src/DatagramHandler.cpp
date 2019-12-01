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
    std::string & body = splitDatagram[1];

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

int DatagramHandler::processDatagramBody(std::string & body)
{
    __int128 numberStorage;

    std::string bodyTmp = body;
    int numPos = 0;
    int numPosAbsolute = 0;
    int numPosArray[26];
    //bodyTmp.erase(std::remove(bodyTmp.begin(), bodyTmp.end(), ' '), bodyTmp.end());
    /* check for every number character if there is */
    /* a stream of 26 numbers in row                */
    while(numPos = bodyTmp.find_first_of("0123456789"))
    {
        int numberCounter = 0;
        int charCounter = 0;
        int isCharacterANumber = 0;
        /* check every character after the first one was        */
        /* detected if they create a 26 number character stream */
        do
        {
            /* omit spaces  */
            if(' ' == bodyTmp[numPos+charCounter])
                continue;

            /* check if character represnt number */
            isCharacterANumber = (int) validateNumber(bodyTmp[numPos+charCounter]);
            charCounter++;

            /* if character is number indeed then save its relative */
            /* position from the string and add number storage      */
            if(isCharacterANumber && (26 >= numberCounter))
            {
                numPosArray[numberCounter] = numPos + charCounter - 1;
                numberCounter++;
                numberStorage *= 10;
                numberStorage += bodyTmp[numPos+charCounter-1] - '0';
            }
            else
            {
                /* stream and of number charcter stream */
                break;
            }

        } while (std::string::npos != charCounter);

        /* determine if required 26 digit character are present */
        if(26 <= numberCounter)
        {
            /* 26 number characters are reached --> success             */
            /* Align number characters position to parameter string body*/
            for(int i=0; i<26; i++)
            {
                numPosArray[i] += numPosAbsolute;
            }
            break;
        }
        else
        {
            /* 26 number characters are not reached --> keep trying      */
            numPosAbsolute += numPos + charCounter;
            bodyTmp = bodyTmp.substr(numPosAbsolute);
        }
    }

    int result = 0;
    /* reached end of string and no number stream was detected */
    if(!bodyTmp.empty())
    {
        result = calculate(numberStorage);
        if(1 == result)
        {
            for(int i = 0; i < 26; i++)
            {
                body[numPosArray[i]] = '1';
            }
        }
    }

    return result;
}

bool DatagramHandler::validateNumber(char character)
{
    return character >= '0' ? (character <= '9' ? true : false) : false;
}

int DatagramHandler::calculate(const __int128 & number)
{
    int result = 0;
    if(number > 0)
    {
        __int128 bit24mask = 1000000000000;
                bit24mask *= 1000000000000;
        __int128 resultNumber = number % bit24mask;
        __int128 fancyNumber = resultNumber * 1000000;
        fancyNumber += 252100;

        int numberToAdd = 0;
        uint64_t splited128IntTo64Bits[2] = {*reinterpret_cast<uint64_t*>(&fancyNumber),
                                             *reinterpret_cast<uint64_t*>(&fancyNumber+8)};
        std::string Digits = std::to_string(splited128IntTo64Bits[0]);

        if(!Digits.empty())
        {
            numberToAdd = Digits[0];
            if(Digits.size() > 1)
                numberToAdd += 10*Digits[1];
        }
        else
        {
            Digits = std::to_string(splited128IntTo64Bits[1]);
            if(!Digits.empty())
            {
                numberToAdd = Digits[0];
                if(Digits.size() > 1)
                    numberToAdd += 10*Digits[1];
            }
            else
            {
                #if LOG_LEVEL > 0
                LOGGER << "FATAL ERROR" << std::endl;
                #endif
                SERVER_ASSERT_MSG(false, "number is empty")
            }
        }

        resultNumber += numberToAdd;
        result = resultNumber % 97;
    }

    return result;
}
