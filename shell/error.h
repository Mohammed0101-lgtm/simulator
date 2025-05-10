#ifndef __SHELL_ERROR_H__
#define __SHELL_ERROR_H__


#include <stdexcept>

class Command_error : public std::exception 
{
		private:
		public:
};

class Unsupported_command : public std::exception {};


#endif // __SHELL_ERROR_H__
