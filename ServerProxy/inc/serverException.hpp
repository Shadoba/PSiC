#ifndef __SERVEREXCEPTION__
#define __SERVEREXCEPTION__

#include <exception>
#include <string>

#define SERVER_ASSERT(cond) serverException::assert(cond, "", __func__, errno, __LINE__, __FILE__);
#define SERVER_ASSERT_MSG(cond, msg) serverException::assert(cond, msg, __func__, errno, __LINE__, __FILE__);

/**
 * @brief Excpetion
 * 
 */
class serverException : public std::exception
{
public:
    /**
     * @brief reurns message of what happened
     * 
     * @return const char* error message
     */
    const char* what() const throw() override;

    /**
     * @brief Get the Errno id
     * 
     * @return int errno id
     */
    int getErrno() const;

    /**
     * @brief Test if requiremnts in condition are met
     * 
     * @param condition bool [IN] condition
     * @param msg cstring [IN] user message
     * @param func cstring [IN] being tested function's name
     * @param err int [IN] unix error id
     * @param line int [IN] line in file
     * @param file cstring [IN] filename where test takes place
     */
    static void assert(bool condition, const char * const msg, const char * const func, int err, int line, const char * const file);

    /**
     * @brief Destroy the server Exception object
     * 
     */
    ~serverException();

protected:
    /**
     * @brief Construct a new server Exception object
     * 
     * @param msg cstring [IN] user message
     * @param func cstring [IN] being tested function's name
     * @param err int [IN] unix error id
     * @param line int [IN] line in file
     * @param file cstring [IN] filename where test takes place
     */
    serverException(const char * const msg, const char * const func, int err, int line, const char * const file);

    /**
     * @brief Deleted default constructor
     * 
     */
    serverException() = delete;

private:
    const int m_Errno;          //<? unix error id
    std::string m_Message;      //<? user message
    const int m_line;           //<? line in file
    std::string m_FunctionName; //<? being tested function's name
    std::string m_file;         //<? filename where test takes place
};

#endif /* __SERVEREXCEPTION__ */