#include "RuleClass.h"


Rule::Rule(std::string serial_rule) {
	std::vector<std::string> toks, &toks_ptr = toks;
	int count;

	count = tok_split(serial_rule, RULE_DELIM, toks_ptr);

	src_IP.low = std::stoi(toks.at(6));
	src_IP.high = std::stoi(toks.at(5));

	dest_IP.low = std::stoi(toks.at(4));
	dest_IP.high = std::stoi(toks.at(3));

	actionType = std::stoi(toks.at(2));
	pri = std::stoi(toks.at(1));
	pktCount = std::stoi(toks.at(0));
}

std::string Rule::serialize() {
	std::string serial_rule;

	serial_rule += std::to_string(src_IP.low) + RULE_DELIM;
	serial_rule += std::to_string(src_IP.high) + RULE_DELIM;

	serial_rule += std::to_string(dest_IP.low) + RULE_DELIM;
	serial_rule += std::to_string(dest_IP.high) + RULE_DELIM;

	serial_rule += std::to_string(actionType) + RULE_DELIM;
	serial_rule += std::to_string(pri) + RULE_DELIM;
	serial_rule += std::to_string(pktCount);

	return serial_rule;
}
