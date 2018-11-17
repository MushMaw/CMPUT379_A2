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

#define PKTSTATS_PRINT_RCV "Received: "
#define PKTSTATS_PRINT_SEND "Transmitted: "

#define CONTSTATS_PRINT_RCV "OPEN:%d, QUERY:%d\n"
#define CONTSTATS_PRINT_SEND "ACK:%d, ADD:%d\n"

#define SWSTATS_PRINT_RCV "ADMIT:%d, ACK:%d, ADDRULE:%d, RELAYIN:%d\n"
#define SWSTATS_PRINT_SEND "OPEN:%d, QUERY:%d, RELAYOUT:%d\n" 

class PktStats {
	private:
		std::map<int, int> rcv_stats;
		std::map<int, int> send_stats;
	public:
		virtual void print();
		virtual void log_rcv(PType ptype);
		virtual void log_send(PType ptype);
}

class ContStats : PktStats {
	public:
		ContStats();
		void print() override;
		void log_rcv(PType ptype) override;
		void log_send(PType ptype) override;
}

class SwStats : PktStats {
	public:
		SwStats();
		void print() override;
		void log_rcv(PType ptype) override;
		void log_send(PType ptype) override;
}

#endif
