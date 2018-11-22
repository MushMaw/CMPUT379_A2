/**
 * CMPUT 379 - Assignment 3
 * File Name: SwClass.cc
 * Student Name: Jacob Bakker
 *
 * Desc
 */

#include "SwClass.h"


Switch::Switch(int id, int swj_id, int swk_id, IP_Range ip_range) : id(id),
								    swj_id(swj_id),
								    swk_id(swk_id),
								    ip_range(ip_range) {}
Switch::Switch(std::string& ser_sw) {
	this->deserialize(ser_sw);
}

/**
 * Function: Switch Constructor
 * -----------------------
 * Initializes Switch from command line arguments.
 *
 * Parameters:
 * 	- argc: Number of arguments.
 *	- argv: Array of arguments.
 * Return Value: None
 * Throws: 
 *	- Sw_Exception
 */
Switch::Switch(int argc, char *argv[]){
	struct stat buffer;
	std::string id_str, swj_id_str, swk_id_str, ip_range_str;
	std::string address_str, portnum_str;
	int portnum;

	if (argc != 8) {throw Sw_Exception(ERR_SW_CL_FORMAT, ERR_SW_CONSTR_FUNC, 0);}
	if (stat(argv[2], &buffer) == -1) {throw Sw_Exception(ERR_TFILE_NOT_FOUND, ERR_SW_CONSTR_FUNC, 0);}
	this->tfile.open(argv[2]);

	// Convert char arguments to string objects
	tfile_name = argv[2];
	id_str = argv[1]; swj_id_str = argv[3]; swk_id_str = argv[4];
	ip_range_str = argv[5];
	address_str = argv[6];
	portnum_str = argv[7];

	try {	
		id = get_sw_val(id_str);

		if (swj_id_str == NULL_PORT) { swj_id = 0; }
		else { swj_id = get_sw_val(swj_id_str); }
	
		if (swk_id_str == NULL_PORT) { swk_id = 0; }
		else { swk_id = get_sw_val(swk_id_str); }

		ip_range = IP_Range(ip_range_str);
		
		this->keep_running = true;
		portnum = str_to_int(portnum_str);
		this->client = new Sw_Client(address_str, portnum);
		this->timer = new Timer();
		this->stats = new SwStats();

	} catch (CS_Skt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_CONSTR_FUNC, e.get_traceback(), e.get_error_code()); }
	  catch (Parse_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_CONSTR_FUNC, e.get_traceback(), e.get_error_code()); }
	  catch (IP_Range_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_CONSTR_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: deserialize
 * -----------------------
 * Sets Switch attributes to those specified in serialized Switch string.
 *
 * Parameters:
 * 	- ser_sw: Serialized Switch.
 * Return Value: None
 * Throws: 
 *	- Sw_Exception
 */
void Switch::deserialize(std::string& ser_sw) {
	std::vector<std::string> toks;
	int tok_count;

	tok_count = tok_split(ser_sw, SW_DELIM, toks);
	if (tok_count != 4) { throw Sw_Exception(ERR_SW_SER_FORMAT, ERR_SW_DESERIALIZE_FUNC, 0); }

	try {
		this->id = str_to_int(toks.at(0));

		if (toks.at(1) == NULL_PORT) { this->swj_id = -1; }
		else { this->swj_id = str_to_int(toks.at(1)); }
		if (toks.at(2) == NULL_PORT) { this->swk_id = -1; }
		else { this->swk_id = str_to_int(toks.at(2)); }

		this->ip_range = IP_Range(toks.at(3));
	} catch (Parse_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_DESERIALIZE_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: serialize
 * -----------------------
 * Serializes Switch object as string.
 *
 * Parameters:
 * 	- ser_sw: Stores serialized Switch.
 * Return Value: None
 * Throws: None
 */
void Switch::serialize(std::string& ser_sw) {
	std::string ser_ip_range;

	ser_sw.clear();

	ser_sw += std::to_string(this->id);
	ser_sw += SW_DELIM;

	if (this->swj_id == 0) { ser_sw += NULL_PORT; }
	else { ser_sw += std::to_string(this->swj_id); }
	ser_sw += SW_DELIM;

	if (this->swk_id == 0) { ser_sw += NULL_PORT; }
	else { ser_sw += std::to_string(this->swk_id); }
	ser_sw += SW_DELIM;

	this->ip_range.serialize(ser_ip_range);
	ser_sw += ser_ip_range;
}

/**
 * Function: poll_ports
 * -----------------------
 * Poll Controller and adjacent Switches (if any) for incoming data.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Switch::poll_ports() {
	struct pollfd * port_pfds_ptr = &(this->port_pfds[0]);
	if (poll(port_pfds_ptr, this->port_pfds.size(), 0) < 0) { throw Sw_Exception(ERR_SW_POLL_FAIL, ERR_SW_POLL_PORTS_FUNC, 0); }
	try {
		this->client->poll_server();
	} catch (CS_Skt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_POLL_PORTS_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: print
 * -----------------------
 * Prints Switch attributes as formatted message to stdout.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Switch::print() {
	std::string ip_range_str;
	this->ip_range.serialize(ip_range_str);
	fprintf(stdout, SW_PRINT_MSG, this->id, this->swj_id, this->swk_id, ip_range_str.c_str());
}

/**
 * Function: send_pkt
 * -----------------------
 * Sends Packet "pkt" to specified "port".
 * A message containing the Packet type and fields is printed to stdout, and the
 * Packet type is logged.
 *
 * Parameters:
 * 	- pkt: Packet to send.
 *	- port: Port to send to (0:Controller, 1:Switch J, 2:Switch K)
 * Return Value: None
 * Throws: 
 *	- Sw_Exception
 */
void Switch::send_pkt(Packet &pkt, SwPort port) {
	try {
		switch(port) {
			case CONT_PORT:
				this->client->send_pkt(pkt);
				break;
			case SWJ_PORT:
				pkt.write_to_fd(this->port_pfds[SWJ_PORT - 1].fd);
				break;
			case SWK_PORT:
				pkt.write_to_fd(this->port_pfds[SWK_PORT - 1].fd);
				break;
			default:
				return;
		}
	} catch (CS_Skt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_SEND_PKT_FUNC, e.get_traceback(), e.get_error_code()); }
	catch (Pkt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_SEND_PKT_FUNC, e.get_traceback(), e.get_error_code()); }

	// Print log of sent packet and record sent packet type in stats
	this->stats->log_send(pkt.ptype);
	this->print_log(pkt, static_cast<int>(port), LOG_RCV_MODE);
}

/**
 * Function: rcv_pkt
 * -----------------------
 * Receieves Packet "pkt" from specified port.
 * This method assumes there is data to be receieved, so the "poll_ports"
 * method should be called first to avoid blocking I/O.
 *
 * Parameters:
 * 	- pkt: Stores the receieved Packet.
 *	- port: Port to receive from (0:Controller, 1:Switch J, 2:Switch K)
 * Return Value: None
 * Throws: None
 */
void Switch::rcv_pkt(Packet &pkt, SwPort port) {
	Header header();

	try {
		switch(port) {
			case CONT_PORT:
				this->client->rcv_pkt(pkt);
				break;
			case SWJ_PORT:
				pkt.read_from_fd(this->port_pfds[SWJ_PORT].fd);
				break;
			case SWK_PORT:
				pkt.read_from_fd(this->port_pfds[SWK_PORT].fd);
				break;
			default:
				return;
		}
	} catch (CS_Skt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_RCV_PKT_FUNC, e.get_traceback(), e.get_error_code()); }
	catch (Pkt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_RCV_PKT_FUNC, e.get_traceback(), e.get_error_code()); }

	// Print log of received packet and record newly-received packet type in stats
	this->stats->log_rcv(pkt.ptype);
	this->print_log(pkt, static_cast<int>(port), LOG_SEND_MODE);
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
void Switch::print_log(Packet &pkt, int sd, LogMode mode) {
	return;
}

/**
 * Function: start
 * -----------------------
 * Sends an OPEN Packet containing the Switch's info to the Controller, then
 * waits for an ACK Packet from the Controller.
 *
 * Parameters: None
 * Return Value: None
 * Throws: 
 *	- Sw_Exception
 */
void Switch::start() {
	Packet open_pkt, ack_pkt;
	std::string ser_sw("");

	this->serialize(ser_sw);
	open_pkt.ptype = PT_OPEN;
	open_pkt.set_msg(ser_sw);

	try {
		this->send_pkt(open_pkt, CONT_PORT);
		while (1) {
			this->poll_ports();
			if (this->client->is_pkt_from_server()) {
				this->client->rcv_pkt(ack_pkt);
				if (ack_pkt.ptype == PT_ACK) { break; }
			}	
		}
	} catch (CS_Skt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_START_FUNC, e.get_traceback(), e.get_error_code()); }
	  catch (Sw_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_START_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: list
 * -----------------------
 * Prints information of all running Switches, then prints stats for
 * received/sent Packets.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Switch::list() {
	int ft_len = this->flow_table.size();

	// Print flow table entries
	std::cout << SW_PRINT_FT_TABLE_TITLE;
	for (int i = 0; i < ft_len; i++) {
		fprintf(stdout, SW_PRINT_FT_TABLE_IDX, i);
		this->flow_table[i]->print();
	}

	// Print packet stats
	this->stats->print();
}

/**
 * Function: read_next_traffic_line
 * -----------------------
 * Reads and handles next line from traffic file, if available.
 * If EOF has been reached or the line is either empty or a comment
 * line (i.e. starts with '#'), the method terminates.
 * If the line is a header whose Switch ID matches that of the this 
 * Switch, the header is either compared against the flow table using
 * the "handle_header" method or - if the header specifies a delay - 
 * the Switch starts a timer delay.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Switch::read_next_traffic_line() {
	Header header;
	std::string line("");
	char c, line_c_str[MAX_LINE_READ];

	// Check if end of traffic file has been reached.
	if (this->tfile.eof()) { return; }

	// If next line is empty or is a comment (starts with '#'), do nothing.
	c = this->tfile.peek();
	this->tfile.getline(line_c_str, MAX_LINE_READ);
	line = line_c_str;
        if (c == '#' || c == '\n') { return; }	

	header.deserialize(line);
	if (header.swi == this->id) {
		if (header.timeout > 0) {
			//TODO: Delay for some time
			this->start_traffic_delay(header.timeout);
		} else {
			this->handle_header(header);
		}
	}
}

/**
 * Function: handle_header
 * -----------------------
 * Attempts to match header to some Rule in flow table.
 * If a matching Rule exists, execute it on the Header.
 * Otherwise, query the Controller for a new Rule and execute
 * the received Rule.
 *
 * Parameters:
 * 	- header: The Header to be matched to a Rule.
 * Return Value: None
 * Throws: None
 */
void Switch::handle_header(Header &header) {
	int ft_len = this->flow_table.size();
	int match_idx = -1;

	for (int i = 0; i < ft_len; i++) {
		if (this->flow_table[i]->is_match(header)) {
			match_idx = i;
			break;
		}
	}

	if (match_idx < 0) { match_idx = this->query_cont(header); }
	this->execute_rule(header, match_idx);
}

/**
 * Function: query_cont
 * -----------------------
 * 
 *
 * Parameters:
 * 	- ser
 * Return Value: None
 * Throws: None
 */
int Switch::query_cont(Header& header) {
	Packet que_pkt, add_pkt;
	Rule * new_rule = NULL;
	std::string que_msg;

	que_pkt.ptype = PT_QUERY;
	header.serialize(que_msg);
	que_pkt.msg = que_msg;

	try {
		this->send_pkt(que_pkt, CONT_PORT);
		while (1) {
			if (this->client->is_pkt_from_server()) {
				this->client->rcv_pkt(add_pkt);
				new_rule = new Rule(add_pkt.msg);
				this->flow_table.push_back(new_rule);
				return this->flow_table.size();
			}
		}
	} catch (Sw_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_QUERY_CONT_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: 
 * -----------------------
 * Ser
 *
 * Parameters:
 * 	- ser
 * Return Value: None
 * Throws: None
 */
void Switch::execute_rule(Header& header, int rule_idx) {
	Rule * rule;
	Packet relay_pkt;
	std::string ser_header;

	try {
		// If action type is FORWARD, send header to specified port
		rule = this->flow_table[rule_idx];
		if (rule->act_type == AT_FORWARD) {
			relay_pkt.ptype = PT_RELAY;
			header.serialize(ser_header);
			relay_pkt.msg = ser_header;
			this->send_pkt(relay_pkt, rule->act_val);
		}
		
		// Increment packet count of rule
		this->flow_table[rule_idx]->pkt_count++;
	} catch (Sw_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_EXECUTE_RULE_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: 
 * -----------------------
 * Ser
 *
 * Parameters:
 * 	- ser
 * Return Value: None
 * Throws: None
 */
void Switch::install_rule(IP_Range src_IP, IP_Range dest_IP, ActType atype, SwPort aval, int pri) {
	Rule * new_rule = new Rule(src_IP, dest_IP, atype, aval, pri);
	this->flow_table.push_back(new_rule);
}

/**
 * Function: 
 * -----------------------
 * Ser
 *
 * Parameters:
 * 	- ser
 * Return Value: None
 * Throws: None
 */
void Switch::start_traffic_delay(int delay) {
	this->timer->start(delay);
}

/**
 * Function: 
 * -----------------------
 * Ser
 *
 * Parameters:
 * 	- ser
 * Return Value: None
 * Throws: None
 */
void Switch::handle_user_cmd() {
	std::string user_cmd("");

	std::cin >> user_cmd;
	if (user_cmd == SW_USER_LIST_CMD) {
		this->list();
	} else if (user_cmd == SW_USER_EXIT_CMD) {
		this->keep_running = false;
	} else {
		std::cout << user_cmd << ERR_SW_INVALID_USER_CMD;
	}
}

/**
 * Function: 
 * -----------------------
 * Ser
 *
 * Parameters:
 * 	- ser
 * Return Value: None
 * Throws: None
 */
void Switch::run() {
	Packet pkt;
	struct pollfd stdin_pfd[1];

	try {
		// Send OPEN packet, then wait for ACK packet
		this->start();

		// Prepare for polling stdin
		stdin_pfd[0].fd = STDIN_FILENO;
		stdin_pfd[0].events = POLLIN;
		stdin_pfd[0].revents = 0;

		while (1) {
			// Handle next line (if any) from traffic file
			// Check if switch is currently delayed before reading
			if (this->timer->at_target_duration() == true) {
				this->read_next_traffic_line();
			}

			// Poll stdin for user command
			poll(stdin_pfd, 1, 0);
			if (stdin_pfd[0].revents & POLLIN) {
				this->handle_user_cmd();
			}

			// If user entered "exit" command, exit run loop
			if (this->keep_running == false) {
				break;
			}

			// Poll adjacent switches
			this->poll_ports();
			if (this->port_pfds[SWJ_PORT].revents & POLLIN) {
				this->rcv_pkt(pkt, SWJ_PORT);
			} else if (this->port_pfds[SWK_PORT].revents & POLLIN) {
				this->rcv_pkt(pkt, SWK_PORT);
			}
		}
	} catch (Sw_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_RUN_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
void Switch::run() {
	int in_fifo, out_fifo, kg = 0;
	std::string ser_sw (""), fifo_name (""), ser_pkt ("");
	Packet *out_pkt = NULL, *in_pkt = NULL;
	struct pollfd fds[1], wr_fd;

	get_fifo_name(fifo_name, this->swk_id, this->id);
	std::cout << "got out fifo name as" << fifo_name << "\n";
	in_fifo = open(fifo_name.c_str(), O_RDONLY | O_NONBLOCK);
	std::cout << "opened read fifo\n";

	get_fifo_name(fifo_name, this->id, this->swk_id);
	out_fifo = open(fifo_name.c_str(), O_WRONLY);
	std::cout << "opended write fifo as" << fifo_name << "\n";

	fds[0].fd = in_fifo;
	fds[0].events = POLLIN;

	this->serialize(ser_sw);
	out_pkt = new Packet(PT_OPEN, ser_sw);
	out_pkt->write_to_fifo(out_fifo);

	while (kg == 0) {
		if (poll(fds, 1, 0) == -1) {std::cout << "poll error\n";}
		if (fds[0].revents & POLLIN) { kg = 1; }
	}

	in_pkt = new Packet();
	in_pkt->read_from_fifo(in_fifo);
	in_pkt->serialize(ser_pkt);
	std::cout << "Received packet: " << ser_pkt;

	delete in_pkt;
}
*/
