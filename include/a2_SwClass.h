/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

#if !defined(A2_SWCLASS_H)
#define A2_SWCLASS_H 1

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>

#include "a2_parselib.h"
#include "a2_constants.h"
#include "a2_RuleClass.h"

#define ACT_FORWARD 1
#define ACT_DROP 2

#define ERR_TFILE_NOT_FOUND "trafficfile not found\n"
#define ERR_SW_VAL "Switch number must be an integer from 1 to 7\n"
#define ERR_IP_RANGE_INVALID "IP values ip_low-ip_high must be positive integers where ip_low < ip_high\n"

enum SwPort {CONT, SWJ, SWK, IP, SWPORT_COUNT};

class Sw_Exception : public std::runtime_error {
	public:
		Sw_Exception(const char* message) : std::runtime_error(message){}
};

class SwInfo {
	private:
		int id, swj_id, swk_id;
		IP_Range ip_range;

	public:
		SwInfo(int id, int swj_id, int swk_id, IP_Range ip_range);
		SwInfo(std::string& ser_sw);
		std::string serialize();
};

class Switch : SwInfo {
	private:
		std::string tfile_name;
		std::vector<Rule> flow_table;
		std::vector<struct pollfd> ports;
	public:
		Switch(int argc, char *argv[]);
		void run();
		
};

#endif
