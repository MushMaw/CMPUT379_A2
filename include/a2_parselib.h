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

#include "a2_constants.h"

#define STR_FIFO_BASE std::string ("fifo-")
#define FIFO_PERMS S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH

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


#endif
