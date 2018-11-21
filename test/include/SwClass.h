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
#include <fcntl.h>

#include "parselib.h"
#include "constants.h"

#include "RuleClass.h"
#include "PktClass.h"
#include "CS_SocketClass.h"
#include "PktStatsClass.h"
#include "TimerClass.h"

#define ACT_FORWARD 1
#define ACT_DROP 2

#define NULL_PORT std::string ("null")
#define SW_DELIM std::string(" ")

#define SW_USER_LIST_CMD std::string("list")
#define SW_USER_EXIT_CMD std::string("exit")

#define SW_PRINT_INFO "[sw%d] port1=%d, port2=%d, port3=%s\n"

#define ERR_INVALID_SW_CMD " is not a valid command\n"
#define ERR_TFILE_NOT_FOUND "trafficfile not found\n"
#define ERR_SW_VAL "Switch number must be an integer from 1 to 7\n"
#define ERR_IP_RANGE_INVALID "IP values ip_low-ip_high must be positive integers where ip_low < ip_high\n"
#define ERR_SW_POLL_FAIL "Polling failed\n"

class Sw_Exception : public Traceback_Exception {
	public:
		Sw_Exception(const char* msg, const std::string cur_func, const std::string func_traceback) 
		: Traceback_Exception(msg, cur_func, func_traceback) {}
		Sw_Exception(const char* msg, const std::string cur_func)
		: Traceback_Exception(msg, cur_func) {}
};

class Switch {
	private:
		std::string tfile_name;
		std::vector<Rule *> flow_table;
		std::vector<struct pollfd> port_pfds;
		std::ifstream tfile;
		bool keep_running;

		Sw_Client * client;
		PktStats * stats;
		Timer * timer;

	public:
		int id, swj_id, swk_id;
		IP_Range ip_range;

		Switch(int argc, char *argv[]);
		Switch(int id, int swj_id, int swk_id, IP_Range ip_range);
		Switch(std::string& ser_sw);

		void serialize(std::string& ser_sw);
		void deserialize(std::string& ser_sw);
		void run();
		
};

#endif
