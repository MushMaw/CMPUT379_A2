#if !defined(HEADER_CLASS_H)
#define HEADER_CLASS_H

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <vector>

#include "parselib.h"

#define HEADER_PRINT_MSG "header= (srcIP= %d, destIP= %d)\n"

#define HEADER_DELIM std::string(" ")
#define HEADER_DELAY_TYPE std::string("delay")

#define ERR_HEADER_DESERIALIZE_FUNC std::string("Header::deserialize()")

#define ERR_HEADER_INVALID_FORMAT "Invalid format for serialized Header\n"


class Header_Exception : public Traceback_Exception {
	public:
		Header_Exception(const char* msg, const std::string cur_func, const std::string func_traceback) 
		: Traceback_Exception(msg, cur_func, func_traceback) {}
		Header_Exception(const char* msg, const std::string cur_func)
		: Traceback_Exception(msg, cur_func) {}
};

class Header {
	public:
		int swi, src_ip, dest_ip, timeout;

		Header(int swi, int src_ip, int dest_ip)
			: swi(swi),
			  src_ip(src_ip),
			  dest_ip(dest_ip) {}
		Header(std::string& ser_header);

		void serialize(std::string& ser_header);
		void deserialize(std::string& ser_header);
		void print();
};

#endif
