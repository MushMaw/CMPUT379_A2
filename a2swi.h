/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

#include <iostream>
#include <exception>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#define ERR_FILE_NOT_FOUND "trafficfile not found\n"
#define ERR_SW_VAL "Switch number must be an integer from 1 to 7\n"
#define ERR_IP_RANGE_INVALID "IP values ip_low-ip_high must be positive integers where ip_low<ip_high\n"

class Switch {
	int sw_num;
	int swj;
	int swk;
	std::vector FT_Entry flow_table;
	struct ip_range ip_range;
}

int create_switch(int argc, char *argv[]);
