#if !defined(IPRANGE_CLASS_H)
#define IPRANGE_CLASS_H 1

#include <string>
#include <exception>

#include "parselib.h"
#include "constants.h"

#define IP_RANGE_DELIM std::string("-")

#define ERR_IP_RANGE_DESERIALIZE_FUNC std::string("IP_Range::serialize(): ")

#define ERR_IP_RANGE_FORMAT "Invalid IP range format\n"

class IP_Range_Exception :  public Traceback_Exception {
	public:
		IP_Range_Exception(const char* msg, const std::string cur_func, const std::string func_traceback) 
		: Traceback_Exception(msg, cur_func, func_traceback) {}
		IP_Range_Exception(const char* msg, const std::string cur_func)
		: Traceback_Exception(msg, cur_func) {}
};

class IP_Range {
	public:
		int low, high;

		IP_Range();
		IP_Range(int low, int high);
		IP_Range(std::string& ser_ip);

		void serialize(std::string& ser_ip);
		void deserialize(std::string& ser_ip);
		bool is_in_range(int ip_val);
};

#endif
