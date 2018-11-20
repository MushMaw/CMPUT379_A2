// TODO: Print messages as they are received from ports

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

	if (argc != (CONT_ARG_COUNT + 1)) {throw Cont_Exception(ERR_CONT_CL_FORMAT);}
	
	try {
		nswitch_str = argv[2];
		nswitch = str_to_int(nswitch_str);

		if (nswitch < 1) { throw Cont_Exception(ERR_NSWITCH_NON_POS); }
		if (nswitch > MAX_NSWITCH) { throw Cont_Exception(ERR_NSWITCH_EXCEED_MAX); }
		this->nswitch = nswitch;

		portnum_str = argv[3];
		this->portnum = str_to_int(portnum_str);

		this->server = new Cont_Server(port_num, nswitch);
	} catch (Parse_Exception& e) { throw Cont_Exception(e.what()); }
	  catch (CS_Skt_Exception& e) { throw Cont_Exception(e.what()); }

	this->stats = new ContStats();
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
 * Throws: None
 */
void Controller::init_switches() {
	int next_ready_sw = -1;	

	std::cout << CONT_WAIT_SW_START;
	while (this->running_sw != this->nswitch) {
		this->server->poll_clients();
		next_ready_sw = this->server->get_next_ready_cl();

		if (next_ready_sw >= 0 && this->running_sw[next_ready_sw] == NULL) {
			this->rcv_pkt(sw_idx);		
		}
	}
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
	// Send Packet through server
	this->server->rcv_pkt(pkt, sw_idx);

	// Handle packet.
	if (pkt.ptype == PT_OPEN) {
		this->open_new_sw(pkt, sw_idx);
	} else if (pkt.ptype == PT_QUERY) {
		this->handle_query(pkt, sw_idx);
	} else {
		throw Cont_Exception(ERR_INVALID_PKT_RCV);
	}

	// Print receieved packet and log its type.
	this->stats->log_rcv(pkt.ptype);
}

/**
 * Function: 
 * -----------------------
 * Returns 
 *
 * Parameters:
 * 	- str: 
 * Return Value:
 * 	- 
 * Throws:
 *	- 
 */
void Controller::send_pkt(Packet& pkt, int sw_idx) {
	this->server->send_pkt(pkt, sw_idx);
	
	this->stats->log_send(pkt.ptype);
}

/**
 * Function: 
 * -----------------------
 * Returns 
 *
 * Parameters:
 * 	- str: 
 * Return Value:
 * 	- 
 * Throws:
 *	- 
 */
void Controller::open_new_sw(Packet &open_pkt, int sw_idx) {
	Packet ack_pkt(PT_ACK, PKT_EMPTY_MSG);
	Switch * new_sw = NULL;

	try {
		new_sw = new Switch();
		this->running_sw[sw_idx] = new_sw;
		this->send_pkt(pkt, sw_idx);
	} catch (Sw_Exception& e) {
		std::cout << e.what();
	}
}

/**
 * Function: 
 * -----------------------
 * Returns 
 *
 * Parameters:
 * 	- str: 
 * Return Value:
 * 	- 
 * Throws:
 *	- 
 */
void Controller::handle_query(Packet &que_pkt, int sw_idx) {
	Header header();
	int dest_sw = -1, dest_sw_idx = -1, act_val;

	Switch * sw;
	Packet add_pkt();
	Rule * rule;
	std::string ser_rule("");
	ActType act_type;
	IP_Range src_ip(), dest_ip();

	header = Header(que_pkt.msg);
	for (int i = 0; i < nswitch; i++) {
		if (i == sw_idx) { continue; }
		sw = this->running_sw[i];
		if (sw->ip_range.is_in_range(header.dest_ip)) {
			dest_sw = sw->id;
			dest_sw_idx = i;
			break;
		}
	}

	if (dest_sw < 0) { 
		act_type = AT_DROP;
		act_val = 0;
       	}
	else {
		act_type = AT_FORWARD;
		if (dest_sw > sw_id) { act_val = SWK_PORT; }
		else { act_val = SWJ_PORT; }
	}

	src_ip = IP_Range(0, MAX_IP);
	dest_ip = IP_Range(sw->ip_range.low, sw->ip_range.high);

	rule = new Rule(src_ip, dest_ip, MIN_PRI, act_type, act_val);
	rule->serialize(ser_rule);
	add_pkt.ptype = PT_ADD;
	add_pkt.msg = ser_rule;
	this->send_pkt(add_pkt, sw_idx);
}	

/**
 * Function: 
 * -----------------------
 * Returns 
 *
 * Parameters:
 * 	- str: 
 * Return Value:
 * 	- 
 * Throws:
 *	- 
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
 * Function: 
 * -----------------------
 * Returns 
 *
 * Parameters:
 * 	- str: 
 * Return Value:
 * 	- 
 * Throws:
 *	- 
 */
void Controller::list() {
	int cur_sw_num = 0, i;
	// List switch info
	fprintf(stdout, CONT_PRINT_SW_INFO_HEADER, this->nswitch);
	while (cur_sw_num != this->nswitch) {
		for (i = 0; i < this->nswitch; i++) {
			if (this->running_sw->swi == cur_sw_num) {
				this->running_sw->print();
				break;
			}
		}
		cur_sw_num++;
	}
	// List packet stats
	this->stats.print();
}

/**
 * Function: 
 * -----------------------
 * Returns 
 *
 * Parameters:
 * 	- str: 
 * Return Value:
 * 	- 
 * Throws:
 *	- 
 */
void Controller::run() {
	int next_ready_sw;
	struct pollfd stdin_pfd[1];	

	// Accept all switch connections, then get for OPEN packets from each
	this->server->accept_clients();
	this->init_switches();

	// Prepare for polling stdin
	stdin_fd[0].fd = STDIN_FILENO;
	stdin_fd[0].events = POLLIN;
	stdin_fd[0].revents = 0;

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
	
		// Poll switches for packets, then handle each incoming packet (if any)
		this->server->poll_clients();
		next_ready_sw = this->server->get_next_ready_cl();
		while (next_ready_sw != -1) {
			this->handle_sw_pkt(next_ready_sw);
			next_ready_sw = this->server->get_next_ready_cl();
		}
	}

	// TODO: Clean up controller stuff if necessary
}
