#if !defined(A2_CONT_CLASS_H)
#define A2_CONT_CLASS_H 1

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>

#include "a2_utility.h"
#include "a2_constants.h"

#define ERR_NSWITCH_NON_POS "nswitch must be a positive value\n"
#define ERR_NSWITCH_EXCEED_MAX "Inputted nswitch exceeds maximum allowed switches\n"

class Cont_Exception : public std::runtime_error {
	public:
		Cont_Exception(const char* message) : std::runtime_error(message){}
};

class Controller {
	private:
		int nswitch, running_sw;
		struct pollfd fds[MAX_NSWITCH];
	public:
		Controller(int argc, char *argv[]);
		int handle_inc_packets();
};

#endif
