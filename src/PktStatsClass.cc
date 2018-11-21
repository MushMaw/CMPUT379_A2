/**
 * CMPUT 379 - Assignment 3
 * File Name: PktStatsClass.cc
 * Student Name: Jacob Bakker
 *
 * Implements Packet Stats Classes for both Controllers and Switches.
 *
 * The parent Packet Stats Class contains methods for logging receieved/sent Packets
 * by Packet type. The two child classes - ContStats and SwStats - implement their
 * own "print" methods for displaying the number of each Packet type receieved/sent
 * according to which Packet types are receieved/sent by the Controller and Switch
 * respectively.
 */

#include "PktStatsClass.h"

/**
 * Constructors for ContStats and SwStats
 */
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

/**
 * Function: log_rcv
 * -----------------------
 * Increments number of receieved packets of type "ptype".
 *
 * Parameters:
 * 	- ptype: Packet type that was receieved.
 * Return Value: None
 * Throws: None
 */
void PktStats::log_rcv(PType ptype) {
	this->rcv_stats[ptype]++;
}

/**
 * Function: log_send
 * -----------------------
 * Increments number of sent packets of type "ptype".
 *
 * Parameters:
 * 	- ptype: Packet type that was sent.
 * Return Value: None
 * Throws: None
 */
void PktStats::log_send(PType ptype) {
	this->send_stats[ptype]++;
}
/**
 * Function: print (ContStats)
 * -----------------------
 * Prints number of received/sent Packets for each Packet type
 * handled by Controller.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void ContStats::print() {
	std::cout << PKTSTATS_PRINT_RCV;
	fprintf(stdout, CONTSTATS_PRINT_RCV, this->rcv_stats[PT_OPEN], this->rcv_stats[PT_QUERY]);
	std::cout << PKTSTATS_PRINT_SEND;
	fprintf(stdout, CONTSTATS_PRINT_SEND, this->send_stats[PT_ACK], this->send_stats[PT_ADD]);
}

/**
 * Function: print (SwStats)
 * -----------------------
 * Prints number of received/sent Packets for each Packet type
 * handled by Switch.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void SwStats::print() {
	std::cout << PKTSTATS_PRINT_RCV;
	fprintf(stdout, SWSTATS_PRINT_RCV, this->rcv_stats[PT_ADMIT], this->rcv_stats[PT_ACK],
					   this->rcv_stats[PT_ADD], this->rcv_stats[PT_RELAY]);
	std::cout << PKTSTATS_PRINT_SEND;
	fprintf(stdout, SWSTATS_PRINT_RCV, this->send_stats[PT_OPEN], this->send_stats[PT_QUERY],
					   this->send_stats[PT_RELAY]);
}
