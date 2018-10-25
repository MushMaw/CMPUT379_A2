#if !defined(A2_RULECLASS_H)
#define A2_RULECLASS_H

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <vector>

#include "a2_utility.h"

#define RULE_DELIM std::string (" ")

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
		std::string serialize();
};

#endif
