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

#define ERR_NSWITCH_NON_POS "nswitch must be a positive value\n"
#define ERR_NSWITCH_EXCEED_MAX "Inputted nswitch exceeds maximum allowed switches\n"

class Cont_Exception : public std::runtime_error {
	public:
		Cont_Exception(const char* message) : std::runtime_error(message){}
};

class Controller {
	private:
		static bool list_sig_caught = false;
		int nswitch, running_sw_count;
		std::vector<struct pollfd *> fds[MAX_NSWITCH];
		std::vector<Switch *> running_sw;
	public:
		Controller(int argc, char *argv[]);
		int handle_inc_packets();
};

#endif
