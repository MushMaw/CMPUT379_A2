#include "PktStatsClass.h"

ContStats::ContStats() {
	this->rcv_stats[PT_OPEN] = 0;
	this->rcv_stats[PT_QUERY] = 0;

	this->send_stats[PT_ACK] = 0;
	this->send_stats[PT_ADD] = 0;
}

SwStats::SwStats() {
	this->rcv_stats[PT_ADMIT] = 0;
	this->rcv_stats[PT_ACK] = 0;
	this->rcv_stats[PT_ADD] = 0;
	this->rcv_stats[PT_RELAY] = 0;

	this->send_stats[PT_OPEN] = 0;
	this->send_stats[PT_QUERY] = 0;
	this->send_stats[PT_RELAY] = 0;
}

void ContStats::print() {
	std::cout << PKTSTATS_PRINT_RCV;
	fprintf(stdout, CONTSTATS_PRINT_RCV, this->rcv_stats[PT_OPEN], this->rcv_stats[PT_QUERY]);
	std::cout << PKTSTATS_PRINT_SEND;
	fprintf(stdout, CONTSTATS_PRINT_SEND, this->send_stats[PT_ACK], this->send_stats[PT_ADD]);
}

void ContStats::log_rcv(PType ptype) {
	this->rcv_stats[ptype]++;
}

void ContStats::log_send(PType ptype) {
	this->send_stats[ptype]++;
}

void SwStats::print() {
	std::cout << PKTSTATS_PRINT_RCV;
	fprintf(stdout, SWSTATS_PRINT_RCV, this->rcv_stats[PT_ADMIT], this->rcv_stats[PT_ACK],
					   this->rcv_stats[PT_ADD], this->rcv_stats[PT_RELAY]);
	std::cout << PKTSTATS_PRINT_SEND;
	fprintf(stdout, SWSTATS_PRINT_RCV, this->send_stats[PT_OPEN], this->send_stats[PT_QUERY],
					   this->send_stats[PT_RELAY]);
}

void SwStats::log_rcv(PType ptype) {
	this->rcv_stats[ptype]++;
}

void SwStats::log_send(PType ptype) {
	this->send_stats[ptype]++;
}
