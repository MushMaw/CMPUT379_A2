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

#define ERR_PKT_READ "Error during read from file descriptor\n"
#define ERR_PKT_WRITE "Error during write to file descriptor\n"

enum PktType {PT_UNINIT, PT_OPEN, PT_ACK, PT_QUERY, PT_ADD, PT_RELAY, PT_ADMIT};
enum ActType {FORWARD, DROP};

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

		void serialize(std::string& ser_pkt);
		void deserialize(std::string& ser_pkt);
		size_t read_from_fd(int fd);
		size_t write_to_fd(int fd);
};

#endif
