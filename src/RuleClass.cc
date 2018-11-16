#include "RuleClass.h"


Rule::Rule(std::string serial_rule) {
	std::vector<std::string> toks, &toks_ptr = toks;
	int count;

	count = tok_split(serial_rule, RULE_DELIM, toks_ptr);

	this->src_IP.low = std::stoi(toks.at(6));
	this->src_IP.high = std::stoi(toks.at(5));

	this->dest_IP.low = std::stoi(toks.at(4));
	this->dest_IP.high = std::stoi(toks.at(3));

	this->actionType = std::stoi(toks.at(2));
	this->pri = std::stoi(toks.at(1));
	this->pktCount = std::stoi(toks.at(0));
}

void Rule::serialize(std::string& ser_rule) {
	std::string serial_rule;

	ser_rule += std::to_string(src_IP.low) + RULE_DELIM;
	ser_rule += std::to_string(src_IP.high) + RULE_DELIM;

	ser_rule += std::to_string(dest_IP.low) + RULE_DELIM;
	ser_rule += std::to_string(dest_IP.high) + RULE_DELIM;

	ser_rule += std::to_string(actionType) + RULE_DELIM;
	ser_rule += std::to_string(pri) + RULE_DELIM;
	ser_rule += std::to_string(pktCount);
}
