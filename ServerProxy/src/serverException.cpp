#include <serverException.hpp>
#include <string.h>

/**
 * @brief Construct a new server Exception::server Exception object
 * 
 * @param msg error message
 * @param err errpr number of errno macro
 * @param line line where error occured
 */
serverException::serverException(const char * const msg, const char * const func, int err, int line, const char * const file) : std::exception(), m_FunctionName(func), m_Errno(err), m_line(line), m_file(file)
{
    m_Message = m_file;
    m_Message += ", line: ";
    m_Message += std::to_string(m_line);
    m_Message += ", function: ";
    m_Message += m_FunctionName;
    m_Message += ", ";
    m_Message += msg;
    m_Message += ", errno: (";
    m_Message += std::to_string(m_Errno);
    m_Message += ") ";
    m_Message += strerror(m_Errno);
}

/**
 * @brief what returned error message
 * 
 */
const char* serverException::what() const throw()
{
    return m_Message.c_str();
}

/**
 * @brief getErrno returns error number
 * 
 */
int serverException::getErrno() const
{
    return m_Errno;
}

/**
 * @brief asserts exception if condition is true
 * 
 * @param condition condition if error occured
 * @param msg message of error
 * @param err error number of errno macro
 * @param line line where error occured
 */
void serverException::assert(bool condition, const char * const msg, const char * const func, int err, int line, const char * const file)
{
    if(condition)
    {
        throw PSiC::serverException(msg, func, err, line, file);
    }
}

/**
 * @brief destructor
 * 
 */
serverException::~serverException()
{}