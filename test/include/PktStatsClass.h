/**
 * CMPUT 379 - Assignment 3
 * File Name: PktStatsClass.h
 * Student Name: Jacob Bakker
 *
 * Implements Packet Stats Classes for both Controllers and Switches.
 *
 * The parent Packet Stats Class contains methods for logging receieved/sent Packets
 * by Packet type. 
 *
 * The two child classes - ContStats and SwStats - implement their
 * own "print" methods for displaying the number of each Packet type receieved/sent
 * according to which Packet types are receieved/sent by the Controller and Switch
 * respectively.
 */

#if !defined(PKT_STATS_CLASS_H)
#define PKT_STATS_CLASS_H 1

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
#include <map>

#include "PktClass.h"

#define PKTSTATS_PRINT_TITLE "Packet Stats:\n"
#define PKTSTATS_PRINT_RCV "Received: "
#define PKTSTATS_PRINT_SEND "Transmitted: "

#define CONTSTATS_PRINT_RCV "OPEN:%d, QUERY:%d\n"
#define CONTSTATS_PRINT_SEND "ACK:%d, ADD:%d\n"

#define SWSTATS_PRINT_RCV "ADMIT:%d, ACK:%d, ADDRULE:%d, RELAYIN:%d\n"
#define SWSTATS_PRINT_SEND "OPEN:%d, QUERY:%d, RELAYOUT:%d\n" 


class ContStats {
	public:
		std::map<int, int> rcv_stats;
		std::map<int, int> send_stats;

		ContStats();
		void print();
		void log_rcv(PktType ptype);
		void log_send(PktType ptype);
};

class SwStats {
	public:
		std::map<int, int> rcv_stats;
		std::map<int, int> send_stats;

		SwStats();
		void print();
		void log_rcv(PktType ptype);
		void log_send(PktType ptype);
};

#endif
