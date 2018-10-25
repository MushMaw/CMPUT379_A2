/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

#ifndef A2_UTILITY_H
#define A2_UTILITY_H 1


#include <iostream>
#include <exception>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#include "a2_constants.h"

struct ip_range {
	int low;
	int high;
};

typedef struct ip_range IP_Range;

int str_to_pos_int(std::string const& str);
int get_sw_val(std::string const& arg);
struct ip_range get_ip_range(std::string const& ip_str);
int open_fifo(int reader, int writer, int mode);
#endif
