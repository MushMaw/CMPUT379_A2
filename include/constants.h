/**
 * CMPUT 379 - Assignment 3
 * File Name: constants.h
 * Student Name: Jacob Bakker
 *
 * This header contains basic constants used by many functions like the maximum number of Switches and
 * minimum priority values for Rules.
 *
 * Contained is an Exception class used as the parent class of all Exception classes in the other files.
 * Traceback Exception is intended to allow for the printing of a function traceback as well as an error
 * code as specified by the function that first detected an error. 
 * Each file that implements this class attaches the names of its function that either threw/caught the
 * exception to the traceback string of the exception.
 */

#if !defined(_A2_CONSTANTS_H)
#define _A2_CONSTANTS_H 1

#define MAX_NSWITCH 7
#define MAX_LINE_READ 100
#define MAX_IP 1000
#define MIN_PRI 4

#define SW_MODE std::string ("sw")
#define CONT_MODE std::string ("cont")
#define NULL_SW_STR std::string ("null")

#define TB_EXC_PRINT_START "Exception Occurred:\n"
#define TB_EXC_TRACEBACK "	Traceback: "
#define TB_EXC_ERROR_MSG "	Error: "
#define TB_EXC_TRACEBACK_DELIM std::string(": ")

#define EMPTY_STR std::string("")

#define ERR_CONT_CL_FORMAT "format: ./a3sdn cont nSwitch portnum\n"
#define ERR_SW_CL_FORMAT "format: ./a3sdn swi trafficFile [swj|null] [swk|null] ip_low-ip_high address portnum\n"

enum SwPort {CONT_PORT, SWJ_PORT, SWK_PORT, IP_PORT};

class Traceback_Exception : public std::runtime_error {
	std::string traceback;
	int error_code;
	public:
		Traceback_Exception(const char* msg, const std::string cur_func, const std::string func_traceback, int error_code) : std::runtime_error(msg) {
			this->traceback = "";
			this->traceback += cur_func;
			this->traceback += TB_EXC_TRACEBACK_DELIM;
			this->traceback += func_traceback;
			this->error_code = error_code;
		}
		Traceback_Exception(const char* msg, const std::string cur_func, int error_code) : std::runtime_error(msg) {
			this->traceback = "";
			this->traceback += cur_func;
			this->error_code = error_code;
		}
		void print_traceback() { 
			std::cerr << TB_EXC_PRINT_START;
			std::cerr << TB_EXC_TRACEBACK << this->traceback << "\n";
			std::cerr << TB_EXC_ERROR_MSG << this->what(); 
		}
		std::string get_traceback() { return this->traceback; }
		int get_error_code() { return this->error_code; }
		
};

#endif
