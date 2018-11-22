/**
 * CMPUT 379 - Assignment 3
 * File Name: ContClass.cc
 * Student Name: Jacob Bakker
 *
 */

#include "ContClass.h"

/**
 * Function: Controller constructor
 * -----------------------
 * Initializes Controller object from command line arguments.
 *
 * Parameters:
 * 	- argc: Number of arguments.
 *	- argv: Array of arguments.
 * Return Value: None
 * Throws:
 *	- Cont_Exception
 */
Controller::Controller(int argc, char *argv[]) {
	int nswitch;
	std::string nswitch_str(""), portnum_str("");

	if (argc != CONT_ARG_COUNT) {throw Cont_Exception(ERR_CONT_CL_FORMAT, ERR_CONT_CONSTR_FUNC, 0);}
	
	try {
		nswitch_str = argv[2];
		nswitch = str_to_int(nswitch_str);

		if (nswitch < 1) { throw Cont_Exception(ERR_NSWITCH_NON_POS, ERR_CONT_CONSTR_FUNC, 0); }
		if (nswitch > MAX_NSWITCH) { throw Cont_Exception(ERR_NSWITCH_EXCEED_MAX, ERR_CONT_CONSTR_FUNC, 0); }
		this->nswitch = nswitch;
		// Set placeholders for expected number of switches
		
		for (int i = 0; i < this->nswitch; i++) { this->running_sw.push_back((Switch *) NULL); }
		this->running_sw_count = 0;

		portnum_str = argv[3];
		this->portnum = str_to_int(portnum_str);

		this->server = new Cont_Server(this->portnum, nswitch);
	} catch (Parse_Exception& e) { throw Cont_Exception(e.what(), ERR_CONT_CONSTR_FUNC, e.get_traceback(), e.get_error_code()); }
	  catch (CS_Skt_Exception& e) { throw Cont_Exception(e.what(), ERR_CONT_CONSTR_FUNC, e.get_traceback(), e.get_error_code()); }

	this->stats = new ContStats();
	this->keep_running = true;
}

/**
 * Function: Controller Destructor
 * -----------------------
 * Deletes all allocated attributes of Controller.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
Controller::~Controller() {
	//std::cout << "Delete server\n";
	delete this->server;
	//std::cout << "Delete stats\n";
	delete this->stats;
	//std::cout << "Delete all sw\n";
	for (int i = 0; i < this->nswitch; i++) {
		delete this->running_sw[i];
		//std::cout << "Closed sw: " << i << "\n";
	}
}

/**
 * Function: init_switches
 * -----------------------
 * Initializes Switches by repeatedly polling for OPEN packets, saving the
 * Switch information from the packet, and responding with an ACK Packet to
 * each switch.
 * The Controller will keep polling from OPEN packets until all Switches are
 * running.
 *
 * Parameters: None 
 * Return Value: None
 * Throws: 
 *	- Cont_Exception
 */
void Controller::init_switches() {
	int next_ready_sw = -1;	

	std::cout << CONT_WAIT_SW_START;
	try {
		while (this->running_sw_count != this->nswitch) {
			this->server->poll_clients();
			next_ready_sw = this->server->get_next_ready_cl();

			if (next_ready_sw >= 0 && this->running_sw[next_ready_sw] == NULL) {
				//std::cout << "Sw" << next_ready_sw << " is ready\n";
				this->rcv_pkt(next_ready_sw);		
			}
		}
	} catch (Cont_Exception& e) { throw Cont_Exception(e.what(), ERR_CONT_INIT_SWI_FUNC, e.get_traceback(), e.get_error_code()); }
	std::cout << CONT_SW_START_DONE;
}

/**
 * Function: rcv_pkt
 * -----------------------
 * Retrieves Packet sent by Switch at index "sw_idx", then calls appropriate function
 * for handling the Packet. 
 * The received Packet is printed to stdout and its type is logged.
 *
 * Parameters:
 * 	- sw_idx: Index of Switch in Controller's list of running Switches
 * Return Value: None
 * Throws:
 *	- Cont_Exception
 */
void Controller::rcv_pkt(int sw_idx) {
	int attempts = 0;
	Packet pkt;

	while (attempts < CONT_MAX_RCV_ATTEMPTS) {
		try {
			// Send Packet through server
			this->server->rcv_pkt(pkt, sw_idx);
			//std::cout << "Packet rec from sw\n";

			// Handle packet.
			if (pkt.ptype == PT_OPEN) {
				this->open_new_sw(pkt, sw_idx);
			} else if (pkt.ptype == PT_QUERY) {
				this->handle_query(pkt, sw_idx);
			} else {
				throw Cont_Exception(ERR_INVALID_PKT_RCV, ERR_CONT_RCV_PKT_FUNC, 0);
			}

			// Print receieved packet and log its type.
			//std::cout << "Just before log step\n";
			this->stats->log_rcv(pkt.ptype);
			this->print_log(pkt, sw_idx, LOG_RCV_MODE);
			//std::cout << "log ok\n";
			return;
		} catch (CS_Skt_Exception& e) {
			attempts++;
		}
	}
	throw Cont_Exception(ERR_CONT_MAX_RCV_ATTEMPTS, ERR_CONT_RCV_PKT_FUNC, 0);
}

/**
 * Function: send_pkt
 * -----------------------
 * Sends packet to the Switch given by "sw_idx".
 *
 * Parameters:
 * 	- pkt: The Packet to be sent.
 *	- sw_idx: The Switch's index in the Controllers list of running Switches.
 * Return Value: None
 * Throws:
 *	- Cont_Exception
 */
void Controller::send_pkt(Packet& pkt, int sw_idx) {
	int attempts = 0;
	while (attempts < CONT_MAX_SEND_ATTEMPTS) {
		try {
			this->server->send_pkt(pkt, sw_idx);
			this->stats->log_send(pkt.ptype);
			this->print_log(pkt, sw_idx, LOG_SEND_MODE);
			return;
		} catch (CS_Skt_Exception& e) {
			attempts++;
		}
	}	
	throw Cont_Exception(ERR_CONT_MAX_SEND_ATTEMPTS, ERR_CONT_SEND_PKT_FUNC, 0);
}

/**
 * Function: print_log
 * -----------------------
 * Prints information about Packet including whether it was received or sent, its source
 * and destination, and the contents of its message.
 *
 * Parameters: 
 *	- pkt: Packet to be printed as log
 *	- sd: Either source or destination of Packet. Its identity is based on the "mode",
 *	      where "send" mode means "sd" = destination.
 *	- mode: Either "receive" or "send" mode
 * Return Value: None
 * Throws: None
 */
void Controller::print_log(Packet& pkt, int sw_idx, LogMode mode) {
	int src = -1, dest = -1, sw_id = 0;
	Header rcv_header;
	Rule * send_rule = NULL;
	
	// Get Switch ID
	sw_id = this->running_sw[sw_idx]->id;

	// Define Packet source and destination.
	if (mode == LOG_RCV_MODE) {
		src = sw_id;
		dest = 0;
	} else {
		src = 0;
		dest = sw_id;
	}
	
	// Print start of log
	print_log_start(src, dest, mode, pkt.ptype);

	// Print contents of Packet
	switch(pkt.ptype) {
		case PT_OPEN:
			this->running_sw[sw_idx]->print(); break;
		case PT_ACK:
			break;
		case PT_QUERY:
			rcv_header.deserialize(pkt.msg);
			rcv_header.print();
			break;
		case PT_ADD:
			send_rule = new Rule(pkt.msg);
			send_rule->print();
			delete send_rule;
			break;
		case PT_RELAY:
			break;
		case PT_UNINIT:
			break;
		case PT_ADMIT:
			break;
	}
}

/**
 * Function: open_new_sw
 * -----------------------
 * Given an OPEN Packet sent by a new Switch and an index "sw_idx",
 * this method will save the new Switch's info to its list of Switches
 * at the specified index. The Switch is then sent an ACK Packet.
 *
 * Parameters:
 * 	- open_pkt: The OPEN Packet sent by the new Switch.
 *	- sw_idx: The index in the Controller's list where the new Switch
 *		  info will be saved.
 * Return Value: None
 * Throws: 
 *	- Cont_Exception
 */
void Controller::open_new_sw(Packet &open_pkt, int sw_idx) {
	Packet ack_pkt(PT_ACK);
	Switch * new_sw = NULL;
	//std::cout << "in open new sw\n";
	try {
		new_sw = new Switch(open_pkt.msg);
		//std::cout << "sw created from pkt\n";
		this->running_sw[sw_idx] = new_sw;
		//std::cout << "sw added to vector\n";
		this->send_pkt(ack_pkt, sw_idx);
		//std::cout << "sent act pkt\n";
		this->running_sw_count++;
		//std::cout << "Print new sw\n";
		//this->running_sw[sw_idx]->print();
	} catch (Sw_Exception& e) { throw Cont_Exception(e.what(), ERR_CONT_OPEN_SW_FUNC, e.get_traceback(), e.get_error_code());
	} catch (Cont_Exception& e) { throw Cont_Exception(e.what(), ERR_CONT_OPEN_SW_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: handle_query
 * -----------------------
 * Given a QUERY Packet from the Switch corresponding to index "sw_idx"
 * in the Controller's list of Switches, this method determines the correct
 * destination of the Header given in the QUERY and sends the Switch a 
 * new Rule indicating where to forward Headers with the given destination IP. (
 *
 * If no other Switch handles the given Header, the Rule is set to drop the Header).
 *
 * Parameters:
 * 	- que_pkt: The QUERY Packet receieved from the Switch.
 *	- sw_idx: The index of the Switch in the Controller's list. 
 * Return Value: None
 * Throws:
 *	- Cont_Exception
 */
void Controller::handle_query(Packet &que_pkt, int sw_idx) {
	Header header;
	int dest_sw = -1, sw_id; //,dest_sw_idx = -1,

	Switch * sw;
	Packet add_pkt;
	Rule * rule;
	std::string ser_rule("");
	ActType act_type;
	SwPort act_val;
	IP_Range src_ip, dest_ip;

	// Get ID of Switch that sent QUERY Packet
	sw_id = this->running_sw[sw_idx]->id;

	// Extract Header from Packet message
	header = Header(que_pkt.msg);

	// Find Switch with IP range that contains Header's dest IP
	for (int i = 0; i < nswitch; i++) {
		if (i == sw_idx) { continue; }
		sw = this->running_sw[i];
		if (sw->ip_range.is_in_range(header.dest_ip)) {
			dest_sw = sw->id;
			//dest_sw_idx = i;
			break;
		}
	}

	// If no Switch handles the dest IP, Rule is to drop the Packet.
	if (dest_sw < 0) { 
		act_type = AT_DROP;
		act_val = CONT_PORT;
		// Set destination IP range to include only the Header's destination IP.
		dest_ip = IP_Range(header.dest_ip, header.dest_ip);
       	}
	// Since Switches are arranged in increasing order, the correct port
	// to forward to is determined by comparing Switch IDs.
	else {
		act_type = AT_FORWARD;
		if (dest_sw > sw_id) { act_val = SWK_PORT; }
		else { act_val = SWJ_PORT; }
		// Set destination IP range to Switch's IP range.
		dest_ip = IP_Range(sw->ip_range.low, sw->ip_range.high);
	}

	// Set source and destination IP Ranges for new rule
	src_ip = IP_Range(0, MAX_IP);
	
	// Create and send new Rule to Switch.
	rule = new Rule(src_ip, dest_ip, act_type, act_val, MIN_PRI);
	rule->serialize(ser_rule);
	add_pkt.ptype = PT_ADD;
	add_pkt.msg = ser_rule;
	try {
		this->send_pkt(add_pkt, sw_idx);
		delete rule;
	} catch (Cont_Exception& e) { throw Cont_Exception(e.what(), ERR_CONT_HANDLE_QUERY_FUNC, e.get_traceback(), e.get_error_code()); }
}	

/**
 * Function: handle_user_cmd
 * -----------------------
 * If user input in stdin is detected, this method will:
 *	- Call the list() method if the user entered "list".
 *	- Set a flag for stopping the Switch if the user entered "exit".
 *	- Output an error message for all other user input.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Controller::handle_user_cmd() {
	std::string user_input;

	std::cin >> user_input;
	if (user_input == CONT_USER_LIST_CMD) {
		this->list();
	} else if (user_input == CONT_USER_EXIT_CMD) {
		this->keep_running = false;
	} else {
		std::cout << user_input << ERR_INVALID_CONT_USER_CMD;
	}
}

/**
 * Function: list 
 * -----------------------
 * Lists all Switches connected to the Controller, then prints the number
 * of Packets sent and receieved for each Packet type.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Controller::list() {
	// List switch info
	fprintf(stdout, CONT_PRINT_SW_INFO_HEADER, this->nswitch);

	for (int i = 0; i < this->nswitch; i++) {
		this->running_sw[i]->print();
	}
	std::cout << "\n";

	// List packet stats
	this->stats->print();
	std::cout << "\n";
}

/**
 * Function: stop
 * -----------------------
 * Closes open connections and file descriptors, then deletes all
 * heap-allocated attributes.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Controller::stop() {
	this->server->close_all();
}

/**
 * Function: run
 * -----------------------
 * Calls methods for initializing connections to Switches, then enters
 * main run loop for Controller.
 * On termination, all connections and open file descriptors are closed.
 *
 * Parameters: None
 * Return Value: None
 * Throws:
 *	- Cont_Exception
 */
void Controller::run() {
	int next_ready_sw;
	struct pollfd stdin_pfd[1];	

	// Accept all switch connections, then get for OPEN packets from each
	try {
		this->server->accept_clients();
		this->init_switches();

		// Prepare for polling stdin
		stdin_pfd[0].fd = STDIN_FILENO;
		stdin_pfd[0].events = POLLIN;
		stdin_pfd[0].revents = 0;

		while(1) {
			// Poll stdin for user command
			poll(stdin_pfd, 1, 0);
			if (stdin_pfd[0].revents & POLLIN) {
				this->handle_user_cmd();
			}		

			// If user entered "exit" command, exit run loop
			if (this->keep_running == false) {
				break;
			}
			/**
			// Poll switches for packets, then handle each incoming packet (if any)
			this->server->poll_clients();
			next_ready_sw = this->server->get_next_ready_cl();
			while (next_ready_sw != -1) {
				this->rcv_pkt(next_ready_sw);
				next_ready_sw = this->server->get_next_ready_cl(); 
			}*/
		}
	} catch (CS_Skt_Exception& e) { throw Cont_Exception(e.what(), ERR_CONT_RUN_FUNC, e.get_traceback(), e.get_error_code()); }
	  catch (Cont_Exception& e) { throw Cont_Exception(e.what(), ERR_CONT_RUN_FUNC, e.get_traceback(), e.get_error_code()); }

	// Close all file descriptors on exit
	std::cout << CONT_EXIT_MSG;
	this->stop();
}
