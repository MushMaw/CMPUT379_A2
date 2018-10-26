#if !defined(A2_PKTCLASS_H)
#define A2_PKTCLASS_H

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>
#include <vector>

#include "a2_parselib.h"

#define PKT_DELIM std::string (" ")

class Packet {
	int swi, src_IP, dest_IP;

	Packet(int swi, int src_IP, int dest_IP);
	Packet(std::string& ser_pkt);
};

#endif
