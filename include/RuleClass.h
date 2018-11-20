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

enum ActType {AT_FORWARD, AT_DROP};

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
		IP_Range src_ip, dest_ip;
		int pri, pkt_count;
		ActType act_type;
		SwPort act_val;

		Rule(std::string serial_rule);
		Rule(IP_Range src_IP, IP_Range dest_IP, int pri, int act_type, int act_val) 
			: src_IP(src_IP),
			  dest_IP(dest_IP),
			  pri(pri),
			  act_type(act_type),
			  act_val(act_val) {}

		void serialize(std::string& ser_rule);
		bool is_match(Header& header);
		void print();
		void add_pkt();
};

#endif
