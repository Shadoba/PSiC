#ifndef __SERVEREXCEPTION__
#define __SERVEREXCEPTION__

#include <exception>
#include <string>

#define SERVER_ASSERT(cond) serverException::assert(cond, "", __func__, errno, __LINE__);
#define SERVER_ASSERT_MSG(cond, msg) serverException::assert(cond, msg, __func__, errno, __LINE__);

namespace PSiC
{
	class serverException : public std::exception
	{
		public: 
            virtual const char* what() const throw();
            virtual int getErrno() const;

            static void assert(bool condition, const char * const msg, const char * const func, int err, int line);

            ~serverException();
		
		protected:
			serverException(const char * const msg, const char * const func, int err, int line);
			serverException() = delete;
		
		private:
			const int m_Errno;
            std::string m_Message;
            const int m_line;
			std::string m_FunctionName;
	};
}

#endif /* __SERVEREXCEPTION__ */