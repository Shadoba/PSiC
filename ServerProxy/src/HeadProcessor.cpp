#include "HeadProcessor.hpp"

#include "Utility.hpp"

HeadProcessor::HeadProcessor(std::string head)
{
    m_head = head;
    m_outputHead = head;
}

std::string HeadProcessor::getOutputHead()
{
    return m_outputHead;
}