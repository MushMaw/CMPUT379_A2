/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

#if !defined(PARSELIB_H)
#define PARSELIB_H 1


#include <iostream>
#include <exception>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "constants.h"
#include "IPRangeClass.h"

#define STR_FIFO_BASE std::string ("fifo-")

#define SW_PRINT_MSG "[sw%d] port1=%d, port2=%d, port3=%s\n"

#define ERR_PARSELIB_GET_SW_VAL_FUNC std::string("get_sw_val()")
#define ERR_PARSELIB_GET_SW_STR_FUNC std::string("get_sw_str()")
#define ERR_PARSELIB_STR_TO_INT_FUNC std::string("str_to_int()")

#define ERR_PARSELIB_NON_POS_SW_VAL "The given Switch value is not positive\n"
#define ERR_PARSELIB_NON_INT_CHAR "Non-int char found\n"
#define ERR_PARSELIB_SW_NUM_FORMAT "Invalid Switch value format\n"


class Parse_Exception : public Traceback_Exception {
	public:
		Parse_Exception(const char* msg, const std::string cur_func, const std::string func_traceback, int error_code) 
		: Traceback_Exception(msg, cur_func, func_traceback, error_code) {}
		Parse_Exception(const char* msg, const std::string cur_func, int error_code)
		: Traceback_Exception(msg, cur_func, error_code) {}
};

int get_sw_val(std::string const& sw_str);
void get_sw_str(int sw_val, std::string& sw_str);

int str_to_int(std::string const& str);
int tok_split(std::string& str, std::string delim, std::vector<std::string>& toks);

void get_fifo_name(std::string& fifo_name, int writer, int reader);

#endif
