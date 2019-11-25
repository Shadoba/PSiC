#pragma once

#include <string>
#include <map>

class HeadProcessor
{
public:
    HeadProcessor(std::string head);
    std::string getOutputHead();

private:
    std::string m_head;
    std::string m_outputHead;
};
