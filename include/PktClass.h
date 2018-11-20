#if !defined(A2_PKTCLASS_H)
#define A2_PKTCLASS_H

#include <fstream>
#include <string>
#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parselib.h"

#define PKT_LEN 127
#define PKT_DELIM std::string (" ")
#define PKT_EMPTY_MSG std::string("")

#define PKT_LOG_RCV_TEXT "Received: "
#define PKT_LOG_SEND_TEXT "Transmitted: "
#define PKT_LOG_SRC_DEST_STR "(src= %s, dest= %s) "

#define PTYPE_STR_OPEN "[OPEN]"
#define PTYPE_STR_ACK "[ACK]"
#define PTYPE_STR_QUERY "[QUERY]"
#define PTYPE_STR_ADD "[ADD]"
#define PTYPE_STR_RELAY "[RELAY]"

#define ERR_PKT_READ "Error during read from file descriptor\n"
#define ERR_PKT_WRITE "Error during write to file descriptor\n"

enum PktType {PT_UNINIT, PT_OPEN, PT_ACK, PT_QUERY, PT_ADD, PT_RELAY, PT_ADMIT};
enum PktLogMode {PKT_LOG_RCV_MODE, PKT_LOG_SEND_MODE};

class Pkt_Exception: public std::runtime_error {
	public:
		Pkt_Exception(const char* message) : std::runtime_error(message){}
}

class Packet {
	public:
		PktType ptype;
		std::string msg;
	
		Packet();
		Packet(PktType ptype, std::string& msg);
		Packet(std::string& pkt);

		void set_msg(std::string& new_msg);
		void serialize(std::string& ser_pkt);
		void deserialize(std::string& ser_pkt);
		size_t read_from_fd(int fd);
		size_t write_to_fd(int fd);
};

#endif
