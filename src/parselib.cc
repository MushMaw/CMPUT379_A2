/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */


#include "parselib.h"


int get_sw_val(std::string const& arg) {
	int sw_num;
	std::string sw_str = SW_MODE, sw_num_substr;
	if (arg.substr(0,2) == sw_str) {
		sw_num_substr = arg.substr(2, arg.length());
		sw_num = str_to_pos_int(sw_num_substr);
		return sw_num;
	}
	throw Parse_Exception(ERR_SW_NUM_FORMAT);
}

int str_to_pos_int(std::string const& str) {
	int output;
	try { output = std::stoi(str); }
	catch (std::invalid_argument& e) {throw Parse_Exception(ERR_STOI_INVALID_ARG);}
	catch (std::out_of_range& e) {throw Parse_Exception(ERR_INT_RANGE);}
	if (output > -1) {return output;}
	throw Parse_Exception(ERR_NOT_POS_INT);
}

IP_Range get_ip_range(std::string const& ip_str) {
	IP_Range ip_range;
	std::string ip_val_str;
	int delim_idx;
	
	ip_range.low = -1;
	ip_range.high = -1;

	if ((delim_idx = ip_str.find("-")) == -1) { throw Parse_Exception(ERR_IP_RANGE_FORMAT); }

	ip_val_str = ip_str.substr(0, delim_idx);
	ip_range.low = str_to_pos_int(ip_val_str);
	ip_val_str = ip_str.substr((delim_idx + 1), ip_str.length());
	ip_range.high = str_to_pos_int(ip_val_str);

	if (ip_range.low < 0 || ip_range.high < 0) { throw Parse_Exception(ERR_IP_RANGE_INT); }

	return ip_range;
}

void serialize_ip_range(std::string& ser_ip, IP_Range ip_range) {
	ser_ip += std::to_string(ip_range.low);
	ser_ip += std::string ("-");
	ser_ip += std::to_string(ip_range.high);
}

int tok_split(std::string& str, std::string delim, std::vector<std::string>& toks) {
	int tok_start = 0, tok_end = -1, count = 0;

	tok_end = str.find(delim);
	while (tok_end >= 0) {
		toks.push_back(str.substr(tok_start, tok_end - tok_start));
		tok_start = tok_end + 1;
		count++;
		tok_end = str.find(delim, tok_start);
	}
	if (tok_end< 0 && (unsigned)tok_start < str.length()) {
		toks.push_back(str.substr(tok_start, str.length() - tok_start));
		count++;
	}
	return count;
}

void get_fifo_name(std::string& fifo_name, int writer, int reader) {
	fifo_name = STR_FIFO_BASE + std::to_string(writer) + std::string ("-") + std::to_string(reader);
}
