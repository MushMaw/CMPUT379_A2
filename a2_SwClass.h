/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>

#include "a2_utility.h"
#include "a2_constants.h"

#define ERR_TFILE_NOT_FOUND "trafficfile not found\n"
#define ERR_SW_VAL "Switch number must be an integer from 1 to 7\n"
#define ERR_IP_RANGE_INVALID "IP values ip_low-ip_high must be positive integers where ip_low < ip_high\n"

class Sw_Exception : public std::runtime_error {
	public:
		Sw_Exception(const char* message) : std::runtime_error(message){}
};

class Switch {
	private:
		int id, swj_id, swk_id;
		std::string tfile_name;
		struct ip_range ip_range;
		FILE *tfile;
	public:
		Switch(int argc, char *argv[]);
};

int run_switch(int argc, char *argv[]);
