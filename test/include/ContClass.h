#if !defined(A2_CONT_CLASS_H)
#define A2_CONT_CLASS_H 1

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>

#include "PktClass.h"
#include "PktStatsClass.h"
#include "HeaderClass.h"
#include "IPRangeClass.h"
#include "CS_SocketClass.h"
#include "SwClass.h"

#include "parselib.h"
#include "constants.h"

#define CONT_ARG_COUNT 3
#define CONT_MAX_SEND_ATTEMPTS 10
#define CONT_MAX_RCV_ATTEMPTS 10

#define CONT_WAIT_SW_START "Waiting for all switches to start..."
#define CONT_SW_START_DONE "All switches have started."

#define CONT_USER_LIST_CMD std::string("list")
#define CONT_USER_EXIT_CMD std::string("exit")

#define CONT_PRINT_SW_INFO_HEADER "Switch information (nSwitch=%d):\n"

#define ERR_CONT_CONSTR_FUNC std::string("Controller:Controller()")
#define ERR_CONT_INIT_SWI_FUNC std::string("Controller:init_switches()")
#define ERR_CONT_RCV_PKT_FUNC std::string("Controller::rcv_pkt()")
#define ERR_CONT_SEND_PKT_FUNC std::string("Controller::send_pkt()")
#define ERR_CONT_OPEN_SW_FUNC std::string("Controller::open_new_sw()")
#define ERR_CONT_HANDLE_QUERY_FUNC std::string("Controller::handle_query()")
#define ERR_CONT_RUN_FUNC std::string("Controller::run()")

#define ERR_INVALID_CONT_USER_CMD " is not a valid command\n"
#define ERR_NSWITCH_NON_POS "nswitch must be a positive value\n"
#define ERR_NSWITCH_EXCEED_MAX "Inputted nswitch exceeds maximum allowed switches\n"
#define ERR_CONT_MAX_SEND_ATTEMPTS "Packet send error: Max attempts reached\n"
#define ERR_CONT_MAX_RCV_ATTEMPTS "Packet receive error: Max attempts reached\n"
#define ERR_INVALID_PKT_RCV "Packet receive error: Unsupported Packet Type\n"

class Cont_Exception : public Traceback_Exception {
	public:
		Cont_Exception(const char* msg, const std::string cur_func, const std::string func_traceback, int error_code) 
		: Traceback_Exception(msg, cur_func, func_traceback, error_code) {}
		Cont_Exception(const char* msg, const std::string cur_func, int error_code)
		: Traceback_Exception(msg, cur_func, error_code) {}
};

class Controller {
	private:
		int nswitch, running_sw_count, portnum;
		std::vector<Switch *> running_sw;
		bool keep_running;

		Cont_Server * server;
		ContStats * stats;

		void init_switches();
		void rcv_pkt(int sw_idx);
		void send_pkt(Packet& pkt, int sw_idx);
		void open_new_sw(Packet &open_pkt, int sw_idx);
		void handle_query(Packet &que_pkt, int sw_idx);
		void handle_user_cmd();
		void list();
		void stop();
		
	public:
		Controller(int argc, char *argv[]);
		void run();
};

#endif
