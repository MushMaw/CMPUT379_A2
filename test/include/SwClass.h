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
#include <ios>

#include "parselib.h"
#include "constants.h"
#include "CS_log.h"

#include "RuleClass.h"
#include "PktClass.h"
#include "CS_SocketClass.h"
#include "PktStatsClass.h"
#include "TimerClass.h"
#include "HeaderClass.h"


#define SW_ARG_COUNT 8

#define NULL_PORT std::string ("null")
#define SW_DELIM std::string(" ")

#define SW_USER_LIST_CMD std::string("list")
#define SW_USER_EXIT_CMD std::string("exit")

#define SW_PRINT_INFO "[sw%d] port1=%d, port2=%d, port3=%s\n"
#define SW_PRINT_FT_TABLE_TITLE "Flow Table:\n"
#define SW_PRINT_FT_TABLE_IDX "[%d]"

#define SW_DELAY_START_MSG "** Entering a delay period of %d msec\n"
#define SW_DELAY_END_MSG "** Delay period ended\n"
#define SW_EXIT_MSG "Shutting down Switch...\n"

#define ERR_SW_CONSTR_FUNC std::string("Switch::Switch()")
#define ERR_SW_DESERIALIZE_FUNC std::string("Switch::deserialize()")
#define ERR_SW_POLL_PORTS_FUNC std::string("Switch::poll_ports()")
#define ERR_SW_SEND_PKT_FUNC std::string("Switch::send_pkt()")
#define ERR_SW_RCV_PKT_FUNC std::string("Switch::rcv_pkt()")
#define ERR_SW_START_FUNC std::string("Switch::start()")
#define ERR_SW_QUERY_CONT_FUNC std::string("Switch::query_cont()")
#define ERR_SW_EXECUTE_RULE_FUNC std::string("Switch::execute_rule()")
#define ERR_SW_RUN_FUNC std::string("Switch::run()")
#define ERR_SW_READ_TFILE_FUNC std::string("Switch::read_next_traffic_line()")
#define ERR_SW_HANDLE_HEADER_FUNC std::string("Switch::handle_header()")

#define ERR_INVALID_SW_CMD " is not a valid command\n"
#define ERR_TFILE_NOT_FOUND "trafficfile not found\n"
#define ERR_SW_SER_FORMAT "Invalid serialized Switch string\n"
#define ERR_SW_VAL "Switch number must be an integer from 1 to 7\n"
#define ERR_SW_POLL_FAIL "Polling failed\n"
#define ERR_SW_INVALID_USER_CMD "Invalid user command entered\n"

class Sw_Exception : public Traceback_Exception {
	public:
		Sw_Exception(const char* msg, const std::string cur_func, const std::string func_traceback, int error_code) 
		: Traceback_Exception(msg, cur_func, func_traceback, error_code) {}
		Sw_Exception(const char* msg, const std::string cur_func, int error_code)
		: Traceback_Exception(msg, cur_func, error_code) {}
};

class Switch {
	private:
		std::string tfile_name;
		std::vector<Rule *> flow_table;
		std::vector<struct pollfd> port_pfds;
		int swj_wr_fifo, swk_wr_fifo;
		FILE* tfile;
		bool keep_running, is_delayed;

		Sw_Client * client;
		SwStats * stats;
		Timer * timer;

	public:
		int id, swj_id, swk_id;
		IP_Range ip_range;

		Switch(int argc, char *argv[]);
		Switch(int id, int swj_id, int swk_id, IP_Range ip_range);
		Switch(std::string& ser_sw);
		~Switch();

		void serialize(std::string& ser_sw);
		void deserialize(std::string& ser_sw);

		void run();
		void stop();
		void start();
		void poll_ports();
		void list();
		void print();
		void read_next_traffic_line();
		void handle_header(Header &header);
		int query_cont(Header& header);
		void execute_rule(Header& header, int rule_idx);
		void install_rule(IP_Range src_IP, IP_Range dest_IP, ActType atype, SwPort aval, int pri);
		void start_traffic_delay(int delay);
		void handle_user_cmd();
		void print_log(Packet& pkt, int sd, LogMode mode);
		void open_adj_sw_fifos();

		void send_pkt(Packet &pkt, SwPort port);
		void rcv_pkt(Packet &pkt, SwPort port);
		
};

#endif
