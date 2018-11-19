/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

#if !defined(A2_PARSELIB_H)
#define A2_PARSELIB_H 1


#include <iostream>
#include <exception>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "constants.h"

#define STR_FIFO_BASE std::string ("fifo-")
#define FIFO_PERMS S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH

#define SW_PRINT_MSG "[sw%d] port1=%d, port2=%d, port3=%s\n"

#define ERR_STOI_INVALID_ARG "Cannot convert value to positive int\n"
#define ERR_INT_RANGE "Stoi arg out of range\n"
#define ERR_NOT_POS_INT "Converted value is not a positive integer\n"

#define ERR_IP_RANGE_FORMAT "IP range format: <int>-<int> (e.g. 100-200)\n"
#define ERR_IP_RANGE_INT "Both values in IP range must be positive integers\n"

class Parse_Exception : public std::runtime_error {
	public:
		Parse_Exception(const char* message) : std::runtime_error(message){}
};

struct ip_range {
	int low;
	int high;
};

typedef struct ip_range IP_Range;

int str_to_pos_int(std::string const& str);
int get_sw_val(std::string const& arg);

IP_Range get_ip_range(std::string const& ip_str);
void serialize_ip_range(std::string& ser_ip, IP_Range ip_range); 

int tok_split(std::string& str, std::string delim, std::vector<std::string>& toks);
void get_fifo_name(std::string& fifo_name, int writer, int reader);

#endif
