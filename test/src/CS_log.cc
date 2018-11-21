#include "CS_log.h"


/**
 * Function: get_src_dest_str
 * -----------------------
 * Get string representation for Packet sender/receiver.
 *
 * Parameters:
 * 	- sr: Sender/receiver of Packet
 *	- sr_str: Stores string representing sender/receiver
 * Return Value: None
 * Throws: None
 */
void get_src_dest_str(int sr, std::string& sr_str) {
	std::string sw_str("");
	if (sr == 0) {
		sr_str.assign(LOG_CONT_STR);
	} else {
		get_sw_str(sr, sw_str);
		sr_str.assign(sw_str);
	}
}

/**
 * Function: print_log
 * -----------------------
 * Prints beginning of log message for Packets including its source and destination and
 * whether it was receieved or sent.
 * 
 * Note that source/destination are represented as numbers from 0 to MAX_NSWITCH.
 * 0 represents the Controller, while 1 to MAX_NSWITCH represent Switches 
 *
 * Parameters:
 * 	- src: Packet sender
 *	- dest: Packet receiver
 *	- mode: Either "receive" or "send" mode
 * Return Value: None
 * Throws: None
 */
void print_log_start(int src, int dest, LogMode mode) {
	std::string src_str(""), dest_str("");

	// Print whether Packet was received or sent
	if (mode == LOG_SEND_MODE) {
		std::cout << LOG_SEND_STR;
	} else {
		std::cout << LOG_RCV_STR;
	}

	// Print Packet source and destination
	get_src_dest_str(src, src_str);
	get_src_dest_str(dest, dest_str);
	fprintf(stdout, LOG_SRC_DEST_STR, src_str.c_str(), dest_str.c_str());
}
