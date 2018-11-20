#if !defined(HEADER_CLASS_H)
#define HEADER_CLASS_H

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <vector>

#define HEADER_PRINT_MSG "header= (srcIP= %d, destIP= %d)\n"

class Header {
	public:
		int swi, src_IP, dest_IP, timeout;

		Header(int swi, int src_IP, int dest_IP)
			: swi(swi),
			  src_IP(src_IP),
			  dest_IP(dest_IP) {}
		Header(std::string& ser_header);
		void serialize(std::string& ser_header);
};

#endif
