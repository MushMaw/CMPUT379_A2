#if !defined(A2_PKTCLASS_H)
#define A2_PKTCLASS_H

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <vector>

#include "a2_parselib.h"

#define PKT_DELIM std::string (" ")

enum PktType {OPEN, ACK, QUERY, ADD, RELAY};
enum ActType {FORWARD, DROP};

class Packet {
	PktType ptype;
	std::string msg;
	
	Packet(PktType ptype, std::string& msg);
	Packet(std::string& pkt);
	void serialize(std::string& ser_pkt);
};

#endif
