#if !defined(A2_CONT_CLASS_H)
#define A2_CONT_CLASS_H 1

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>

#include "parselib.h"
#include "constants.h"

#define CONT_ARG_COUNT 3
#define CONT_MAX_SEND_ATTEMPTS 10
#define CONT_MAX_RCV_ATTEMPTS 10

#define CONT_WAIT_FOR_SW_START "Waiting for all switches to start..."
#define CONT_SW_START_DONE "All switches have started."

#define CONT_USER_LIST_CMD std::string("list")
#define CONT_USER_EXIT_CMD std::string("exit")

#define CONT_PRINT_SW_INFO_HEADER "Switch information (nSwitch=%d):\n"

#define ERR_INVALID_CONT_USER_CMD " is not a valid command\n"
#define ERR_NSWITCH_NON_POS "nswitch must be a positive value\n"
#define ERR_NSWITCH_EXCEED_MAX "Inputted nswitch exceeds maximum allowed switches\n"
#define ERR_CONT_MAX_SEND_ATTEMPTS "Packet send error: Max attempts reached\n"
#define ERR_CONT_MAX_RCV_ATTEMPTS "Packet receive error: Max attempts reached\n"

class Cont_Exception : public std::runtime_error {
	public:
		Cont_Exception(const char* message) : std::runtime_error(message){}
};

class Controller {
	private:
		int nswitch, running_sw_count;
		std::vector<Switch *> running_sw;

		Cont_Server * server;
		ContStats * stats;

	public:
		Controller(int argc, char *argv[]);
		int handle_inc_packets();
};

#endif
