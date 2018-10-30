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

#include "a2_parselib.h"

#define PKT_LEN 128
#define PKT_DELIM std::string (" ")

enum PktType {PT_OPEN, PT_ACK, PT_QUERY, PT_ADD, PT_RELAY};
enum ActType {FORWARD, DROP};

class Packet {
	public:
		PktType ptype;
		std::string msg;
	
		Packet(PktType ptype, std::string& msg);
		Packet(std::string& pkt);

		void serialize(std::string& ser_pkt);
		void deserialize(std::string& ser_pkt);
		void read_from_fifo(std::ifstream fifo);
		void write_to_fifo(std::ofstream fifo);
};

#endif
