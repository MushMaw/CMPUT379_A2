/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */


#include "a2_utility.h"

int get_sw_val(std::string const& arg) {
	int sw_num;
	std::string sw_str = SW_MODE, sw_num_substr;
	if (arg.substr(0,2) == sw_str) {
		sw_num_substr = arg.substr(2, arg.length());
		sw_num = str_to_pos_int(sw_num_substr);
		return sw_num;
	}
	return -1;
}

int str_to_pos_int(std::string const& str) {
	int output;
	try { output = std::stoi(str); }
	catch (std::invalid_argument& e) {return -1;}
	catch (std::out_of_range& e) {return -1;}
	if (output > -1) {return output;}
	return -1;
}

IP_Range get_ip_range(std::string const& ip_str) {
	IP_Range ip_range;
	std::string ip_val_str;
	int delim_idx;
	
	ip_range.low = -1;
	ip_range.high = -1;

	if ((delim_idx = ip_str.find("-")) == -1) { std::cout << "nodash\n";return ip_range; }

	ip_val_str = ip_str.substr(0, delim_idx);
	ip_range.low = str_to_pos_int(ip_val_str);
	ip_val_str = ip_str.substr((delim_idx + 1), ip_str.length());
	ip_range.high = str_to_pos_int(ip_val_str);

	return ip_range;
}

std::string get_fifo_name(int reader, int writer) {
	std::string fifo_name = STR_FIFO_BASE + std::to_string(writer) + std::string ("-") + std::to_string(reader);
	return fifo_name;
}

int create_fifo(int reader, int writer) {
	int new_fifo;
	std::string fifo_name = get_fifo_name(reader, writer);

	new_fifo = mkfifo(fifo_name.c_str(), FIFO_PERMS);
	return new_fifo;
}

void delete_fifo(int reader, int writer) {
	std::string fifo_name = get_fifo_name(reader, writer);
	unlink(fifo_name.c_str());
}
