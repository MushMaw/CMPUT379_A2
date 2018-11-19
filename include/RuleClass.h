#if !defined(A2_RULECLASS_H)
#define A2_RULECLASS_H

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <vector>

#include "parselib.h"

#define RULE_DELIM std::string (" ")
#define HEADER_DELIM std::string(" ")

class Rule_Exception : public std::runtime_error {
	public:
		Rule_Exception(const char* message) : std::runtime_error(message){}
};

class Header_Exception : public std::runtime_error {
	public:
		Header_Exception(const char* message) : std::runtime_error(message){}
};

class Rule {
	public:
		IP_Range src_IP, dest_IP;
		int pri, pktCount, actionType;

		Rule(std::string serial_rule);
		Rule(IP_Range src_IP, IP_Range dest_IP, int pri, int pktCount, int actionType) 
			: src_IP(src_IP),
			  dest_IP(dest_IP),
			  pri(pri),
			  pktCount(pktCount),
			  actionType(actionType) {}
		void serialize(std::string& ser_rule);
};

class Header {
	public:
		int swi, src_IP, dest_IP;

		Header(int swi, int src_IP, int dest_IP)
			: swi(swi),
			  src_IP(src_IP),
			  dest_IP(dest_IP) {}
		Header(std::string& ser_header);
		void serialize(std::string& ser_header);
};

#endif
