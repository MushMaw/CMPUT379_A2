#include "a2_RuleClass.h"


Rule::Rule(std::string serial_rule) {
	std::vector<std::string> toks;
	int tok_start = 0, tok_end = -1;

	tok_end = serial_rule.find(RULE_DELIM);
	while (tok_end >= 0) {
		toks.push_back(serial_rule.substr(tok_start, tok_end));
		tok_start = tok_end + 1;
		tok_end = serial_rule.find(RULE_DELIM, tok_start);
	}

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
