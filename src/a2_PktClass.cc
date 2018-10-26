#include "a2_PktClass.h"

Packet::Packet(int swi, int src_IP, int dest_IP) : swi(swi),
						   src_IP(src_IP),
						   dest_IP(dest_IP) {}

Packet::Packet(std::string& ser_pkt) {

}
