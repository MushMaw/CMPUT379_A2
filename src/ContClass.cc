// TODO: Print messages as they are received from ports

#include "ContClass.h"


Controller::Controller(int argc, char *argv[]) {
	int nswitch, portnum;
	std::string nswitch_str;

	if (argc != (CONT_ARG_COUNT + 1)) {throw Cont_Exception(ERR_CONT_CL_FORMAT);}
	
	nswitch_str = argv[2];
	nswitch = str_to_pos_int(nswitch_str);

	if (nswitch < 1) {throw Cont_Exception(ERR_NSWITCH_NON_POS);}
	if (nswitch > MAX_NSWITCH) {throw Cont_Exception(ERR_NSWITCH_EXCEED_MAX);}
	this->nswitch = nswitch;

	portnum_str = argv[3];
	portnum = str_to_pos_int(portnum_str);

	try:
		this->server = new Cont_Server(port_num, nswitch);
	catch (CS_Skt_Exception& e) { throw Cont_Exception(e.what(); }

	this->stats = new ContStats();
}

void Controller::init_switches() {
	int next_ready_sw = -1;	

	std::cout << CONT_WAIT_SW_START;
	while (this->running_sw != this->nswitch) {
		this->server->poll_clients();
		next_ready_sw = this->server->get_next_ready_cl();

		if (next_ready_sw >= 0 && this->running_sw[next_ready_sw] == NULL) {
			open_new_sw(next_ready_sw);			
		}
	}
	std::cout << CONT_SW_START_DONE;
}

void Controller::rcv_pkt(int sw_idx) {
	Packet pkt();

	this->server->rcv_pkt(pkt, sw_idx);
	if (pkt.ptype == PT_OPEN) {
		this->open_new_sw(pkt, sw_idx);
	} else if (pkt.ptype == PT_QUERY) {
		this->handle_query(pkt, sw_idx);
	} else {
		throw Cont_Exception(ERR_INVALID_PKT_RCV);
	}

	this->stats->log_rcv(pkt.ptype);
}

void Controller::send_pkt(Packet& pkt, int sw_idx) {
	this->server->send_pkt(pkt, sw_idx);
	this->stats->log_send(pkt.ptype);
}

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

void Controller::handle_query(Packet &que_pkt, int sw_idx) {
	// TODO: Handle query
}	

void Controller::handle_user_cmd() {
	std::string user_input;

	std::cin >> user_input;
	if (user_input == CONT_USER_LIST_CMD) {
		this->list();
	} else if (user_input == CONT_USER_EXIT_CMD) {
		this->keep_running = false;
	} else {
		std::cout << user_input << ERR_INVALID_USER_CMD;
	}
}

void Controller::list() {
	// TODO: List switch info
	
	// List packet stats
	this->stats.print();
}

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
