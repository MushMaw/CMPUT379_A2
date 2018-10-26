#include "a2_PktClass.h"

Packet::Packet(int swi, int src_IP, int dest_IP) : swi(swi),
						   src_IP(src_IP),
						   dest_IP(dest_IP) {}

Packet::Packet(std::string& ser_pkt) {
	std::vector<std::string> toks;
	int count;

	count = tok_split(ser_pkt, PKT_DELIM, toks);

	swi = std::stoi(toks.at(2));
	src_IP = std::stoi(toks.at(1));
	dest_IP = std::stoi(toks.at(1));
}

std::string Packet::serialize() {
	std::string ser_pkt;

	ser_pkt = std::to_string(swi) + PKT_DELIM;
	ser_pkt += std::to_string(src_IP) + PKT_DELIM;
	ser_pkt += std::to_string(dest_IP);

	return ser_pkt;
}
