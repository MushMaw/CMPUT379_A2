/**
 * CMPUT 379 - Assignment 3
 * File Name: HeaderClass.h
 * Student Name: Jacob Bakker
 *
 * Implements class for representing Headers as read from a traffic file 
 * (e.g. The lines "sw4 100 200" and "sw5 delay 500" are both considered
 * headers.
 *
 * When a Header object is created from a traffic file line, it stores either
 * the source and destination IP values or the timeout value depending on the line
 * type, allowing the Switch to simply create a Header and check whether the timeout
 * attribute is defined to determine what type of traffic file line it read.
 *
 * Implements functions for serializing/deserializing Headers to be sent/received
 * by Controllers or Switches.
 */

#if !defined(HEADER_CLASS_H)
#define HEADER_CLASS_H

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <vector>

#include "parselib.h"

#define HEADER_PRINT_MSG "header= (srcIP= %d, destIP= %d)\n"

#define HEADER_DELIM std::string(" ")
#define HEADER_DELAY_TYPE std::string("delay")

#define ERR_HEADER_DESERIALIZE_FUNC std::string("Header::deserialize()")

#define ERR_HEADER_INVALID_FORMAT "Invalid format for serialized Header\n"


class Header_Exception : public Traceback_Exception {
	public:
		Header_Exception(const char* msg, const std::string cur_func, const std::string func_traceback, int error_code) 
		: Traceback_Exception(msg, cur_func, func_traceback, error_code) {}
		Header_Exception(const char* msg, const std::string cur_func, int error_code)
		: Traceback_Exception(msg, cur_func, error_code) {}
};

class Header {
	public:
		int swi, src_ip, dest_ip, timeout;

		Header();
		Header(int swi, int src_ip, int dest_ip)
			: swi(swi),
			  src_ip(src_ip),
			  dest_ip(dest_ip) {}
		Header(std::string& ser_header);

		void serialize(std::string& ser_header);
		void deserialize(std::string& ser_header);
		void print();
};

#endif
