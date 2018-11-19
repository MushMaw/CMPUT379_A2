#include "RuleClass.h"


Rule::Rule(std::string serial_rule) {
	std::vector<std::string> toks, &toks_ptr = toks;
	int count;

	count = tok_split(serial_rule, RULE_DELIM, toks_ptr);
	if (count != 7) { throw Rule_Exception(ERR_RULE_TOK_COUNT); }

	this->src_IP.low = std::stoi(toks.at(0));
	this->src_IP.high = std::stoi(toks.at(1));

	this->dest_IP.low = std::stoi(toks.at(2));
	this->dest_IP.high = std::stoi(toks.at(3));

	this->actionType = std::stoi(toks.at(4));
	this->pri = std::stoi(toks.at(5));
	this->pktCount = std::stoi(toks.at(6));
}

void Rule::serialize(std::string& ser_rule) {
	ser_rule += std::to_string(src_IP.low) + RULE_DELIM;
	ser_rule += std::to_string(src_IP.high) + RULE_DELIM;

	ser_rule += std::to_string(dest_IP.low) + RULE_DELIM;
	ser_rule += std::to_string(dest_IP.high) + RULE_DELIM;

	ser_rule += std::to_string(actionType) + RULE_DELIM;
	ser_rule += std::to_string(pri) + RULE_DELIM;
	ser_rule += std::to_string(pktCount);
}

bool Rule::is_match(Header& header) {
	if (header.src_IP < this->src_IP.low || this->src_IP.high < header.src_IP) {
		return false;
	}
	if (header.dest_IP < this->dest_IP.low || this->dest_IP.high < header.dest_IP) {
		return false;
	}
	return true;
}

void Rule::print() {
	std::string src_ip_str(""), dest_ip_str("");
	std::string action_str;

	// TODO: Get str for action type and KEEP GOING!!! :D
	if (this->a
	fprintf(stdout, RULE_PRINT_MSG, src_ip_str, dest_ip_str,  
}

Header::Header(std::string& ser_header) {
	std::vector<std::string> toks, &toks_ptr = toks;
	int count;

	count = tok_split(ser_header, HEADER_DELIM, toks_ptr);
	if (count != 3) { throw Header_Exception(ERR_HEADER_TOK_COUNT); }

	try {
		this->sw = get_sw_val(toks.at(0));
		this->src_IP = str_to_pos_int(toks.at(1));
		this->dest_IP = str_to_pos_int(toks.at(2));
	} catch (Parse_Exception& e) { throw Header_Exception(e.what()); }
}

void Header::serialize(std::string& ser_header) {
	ser_header += std::to_string(this-sw) + HEADER_DELIM;
	ser_header += std::to_string(this->src_IP) + HEADER_DELIM;
	ser_header += std::to_string(this->dest_IP);
}
