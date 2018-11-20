#include "RuleClass.h"


Rule::Rule(std::string serial_rule) {
	std::vector<std::string> toks, &toks_ptr = toks;
	int count;

	count = tok_split(serial_rule, RULE_DELIM, toks_ptr);
	if (count != 7) { throw Rule_Exception(ERR_RULE_TOK_COUNT); }

	this->src_ip.low = std::stoi(toks.at(0));
	this->src_ip.high = std::stoi(toks.at(1));

	this->dest_ip.low = std::stoi(toks.at(2));
	this->dest_ip.high = std::stoi(toks.at(3));

	this->actionType = std::stoi(toks.at(4));
	this->pri = std::stoi(toks.at(5));
	this->pkt_count = std::stoi(toks.at(6));
}

void Rule::serialize(std::string& ser_rule) {
	ser_rule += std::to_string(src_ip.low) + RULE_DELIM;
	ser_rule += std::to_string(src_ip.high) + RULE_DELIM;

	ser_rule += std::to_string(dest_ip.low) + RULE_DELIM;
	ser_rule += std::to_string(dest_ip.high) + RULE_DELIM;

	ser_rule += std::to_string(actionType) + RULE_DELIM;
	ser_rule += std::to_string(pri) + RULE_DELIM;
	ser_rule += std::to_string(pktCount);
}

bool Rule::is_match(Header& header) {
	if (header.src_ip < this->src_ip.low || this->src_ip.high < header.src_ip) {
		return false;
	}
	if (header.dest_ip < this->dest_ip.low || this->dest_ip.high < header.dest_ip) {
		return false;
	}
	return true;
}

void Rule::add_pkt() {
	this->pkt_count++;
}

void Rule::print() {
	std::string src_ip_str(""), dest_ip_str("");
	std::string action_str;

	std::cout << "(";
	// Print source and destination IP ranges
	fprintf(stdout, RULE_PRINT_IP_RANGES, src_ip_str, dest_ip_str);
	// Print action type with port number for FORWARD types
	if (this->act_type == AT_FORWARD) {
		fprintf(stdout, RULE_PRINT_FORWARD_TYPE, this->forward_port);
	} else {
		std::cout << RULE_PRINT_DROP_TYPE;
	}
	// Print priority and packet count
	fprintf(stdout, RULE_PRINT_PRIORITY_PKT_COUNT, this->priority, this->pkt_count);
	std::cout << ")\n";
}
