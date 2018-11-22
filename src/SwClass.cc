/**
 * CMPUT 379 - Assignment 3
 * File Name: SwClass.cc
 * Student Name: Jacob Bakker
 *
 * Implements a Switch class as part of a linear SDN system.
 *
 * The Switch communicates with the Controller via a TCP Server and
 * with other Switches via FIFOs. Communication operations are non-
 * blocking. Messages are transmitted using the Packet class.
 *
 * On startup, the Switch will send its information (e.g. ID, neighbor 
 * Switch IDs, served IP range) to the Controller. Once acknowledged, the
 * Switch will read from a specified traffic file and handle its specific
 * headers either by admitting them or forwarding them to the appropriate
 * Switch that serves the header's destination IP value.
 *
 * All Packets that are sent/received are outputted to stdout as a 
 * log message describing its source, destination, type, and contents.
 * The number of sent/receieved Packets, in addition to admitted Headers, can
 * be displayed using the appropriate user command via stdin.
 */

#include "SwClass.h"


/**
 * Switch Constructors
 */
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

	if (argc != SW_ARG_COUNT) {throw Sw_Exception(ERR_SW_CL_FORMAT, ERR_SW_CONSTR_FUNC, 0);}
	
	try {
		// Verify that traffic file exists and open it if it does.
		if (stat(argv[2], &buffer) == -1) {throw Sw_Exception(ERR_TFILE_NOT_FOUND, ERR_SW_CONSTR_FUNC, 0);}
		this->tfile = fopen(argv[2], "r");

		// Convert char arguments to string objects
		this->tfile_name = argv[2];
		id_str = argv[1]; swj_id_str = argv[3]; swk_id_str = argv[4];
		ip_range_str = argv[5];
		address_str = argv[6];
		portnum_str = argv[7];
	
		// Get this Switch's ID and IDs of adjacent Switches (or NULL if they don't exist)
		id = get_sw_val(id_str);

		if (swj_id_str == NULL_PORT) { swj_id = -1; }
		else { swj_id = get_sw_val(swj_id_str); }
	
		if (swk_id_str == NULL_PORT) { swk_id = -1; }
		else { swk_id = get_sw_val(swk_id_str); }

		// Get IP range of Switch
		ip_range = IP_Range(ip_range_str);
		
		// Initialize running and delay flags
		this->keep_running = true;
		this->is_delayed = false;

		// Initialize connection to Controller server
		portnum = str_to_int(portnum_str);
		this->client = new Sw_Client(address_str, portnum);

		this->timer = new Timer();
		this->stats = new SwStats();

	} catch (CS_Skt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_CONSTR_FUNC, e.get_traceback(), e.get_error_code()); }
	  catch (Parse_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_CONSTR_FUNC, e.get_traceback(), e.get_error_code()); }
	  catch (IP_Range_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_CONSTR_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: Switch destructor
 * -----------------------
 * Deletes all allocated attributes of Switch.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
Switch::~Switch() {
	int ft_len = this->flow_table.size();

	for (int i = 0; i < ft_len; i++) {
		delete this->flow_table[i];
	}

	delete this->client;
	delete this->timer;
	delete this->stats;
}

/**
 * Function: deserialize
 * -----------------------
 * Sets Switch attributes to those specified in serialized Switch string.
 * This function is intended for use with sending OPEN Packets containing a Switch's
 * info to the Controller.
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
		// Get this Switch's ID and those of adjacent Switches (if any)
		this->id = str_to_int(toks.at(0));

		if (toks.at(1) == NULL_PORT) { this->swj_id = -1; }
		else { this->swj_id = str_to_int(toks.at(1)); }
		if (toks.at(2) == NULL_PORT) { this->swk_id = -1; }
		else { this->swk_id = str_to_int(toks.at(2)); }

		this->ip_range = IP_Range(toks.at(3));

		// Initialize all pointer attributes to NULL to prevent errors on Switch delete
		this->client = NULL;
		this->stats = NULL;
		this->timer = NULL;
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
	// Poll adjacent switches (if any)
	if (poll(port_pfds_ptr, this->port_pfds.size(), 0) < 0) { throw Sw_Exception(ERR_SW_POLL_FAIL, ERR_SW_POLL_PORTS_FUNC, 0); }

	// Poll controller
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
				pkt.write_to_fd(this->swj_wr_fifo);
				break;
			case SWK_PORT:
				pkt.write_to_fd(this->swk_wr_fifo);
				break;
			default:
				return;
		}

		// Print log of sent packet and record sent packet type in stats
		this->stats->log_send(pkt.ptype);
		this->print_log(pkt, static_cast<int>(port), LOG_SEND_MODE);
	} catch (CS_Skt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_SEND_PKT_FUNC, e.get_traceback(), e.get_error_code()); }
	catch (Pkt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_SEND_PKT_FUNC, e.get_traceback(), e.get_error_code()); }
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
				pkt.read_from_fd(this->port_pfds[0].fd);
				break;
			case SWK_PORT:
				pkt.read_from_fd(this->port_pfds[1].fd);
				break;
			default:
				return;
		}
		// Print log of received packet and record newly-received packet type in stats
		this->stats->log_rcv(pkt.ptype);
		this->print_log(pkt, static_cast<int>(port), LOG_RCV_MODE);
	} catch (CS_Skt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_RCV_PKT_FUNC, e.get_traceback(), e.get_error_code()); }
	catch (Pkt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_RCV_PKT_FUNC, e.get_traceback(), e.get_error_code()); }
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
	int src = -1, dest = -1, rule_idx = 0;
	std::string src_str(""), dest_str("");
	Header header;

	// Define packet source and destination
	if (mode == LOG_SEND_MODE) {
		src = this->id;
		dest = sd;
	} else {
		src = sd;
		dest = this->id;
	}
	// Print start of log
	print_log_start(src, dest, mode, pkt.ptype);

	// Print contents of Packet
	switch(pkt.ptype) {
		case PT_OPEN:
			this->print();
			break;
		case PT_ACK:
			break;
		case PT_QUERY:
			header.deserialize(pkt.msg);
			header.print();
			break;
		case PT_ADD:
			rule_idx = this->flow_table.size() - 1;
			this->flow_table[rule_idx]->print();
			break;
		case PT_RELAY:
			header.deserialize(pkt.msg);
			header.print();
			break;
		case PT_ADMIT:
			break;
		case PT_UNINIT:
			break;
	}
	std::cout << "\n";
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
	IP_Range init_src(0, MAX_IP);

	// Prepare OPEN pkt
	this->serialize(ser_sw);
	open_pkt.ptype = PT_OPEN;
	open_pkt.set_msg(ser_sw);

	try {
		this->send_pkt(open_pkt, CONT_PORT);
		// Wait for ACK Packet from Controller
		while (1) {
			this->poll_ports();
			if (this->client->is_pkt_from_server()) {
				this->rcv_pkt(ack_pkt, CONT_PORT);
				if (ack_pkt.ptype == PT_ACK) { break; }
			}	
		}
		// Install initial rule for this Switch
		this->install_rule(init_src, this->ip_range, AT_FORWARD, IP_PORT, MIN_PRI);

		// Open read/write FIFO's to adjacent Switch's (if any)
		this->open_adj_sw_fifos();
		
	} catch (CS_Skt_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_START_FUNC, e.get_traceback(), e.get_error_code()); }
	  catch (Sw_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_START_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: open_adj_sw_fifos
 * -----------------------
 * Opens read/write FIFOs for swj and swk and prepares for polling of read
 * FIFO's of each. 
 * If swj and/or swk do not exist, the poll structures must still be created
 * for the function "poll_ports" to work. However, the "fd" value is set to
 * prevent the polling from ever indicating incoming data.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Switch::open_adj_sw_fifos() {
	struct pollfd sw_pfd;
	std::string fifo_name;

	sw_pfd.fd = -1;
	sw_pfd.events = POLLIN;
	sw_pfd.revents = 0;

	// Init read/write fifos for Switch J and K, and prepare both for polling
	// Init Switch J
	
	if (this->swj_id == -1) {
		this->swj_wr_fifo = -1;
		sw_pfd.fd = -1;
		this->port_pfds.push_back(sw_pfd);
	} else {
		// Open FIFO for reading
		get_fifo_name(fifo_name, this->swj_id, this->id);
		sw_pfd.fd = open(fifo_name.c_str(), O_RDONLY | O_NONBLOCK);
		this->port_pfds.push_back(sw_pfd);

		// Keep attempting to open write FIFO until Switch J opens it in read mode
		while (1) {
			get_fifo_name(fifo_name, this->id, this->swj_id);
			this->swj_wr_fifo = open(fifo_name.c_str(), O_WRONLY  | O_NONBLOCK);
			if (this->swj_wr_fifo != -1) { break; }
		}
	}

	// Init Switch K
	if (this->swk_id == -1) {
		this->swk_wr_fifo = -1;
		sw_pfd.fd = -1;
		this->port_pfds.push_back(sw_pfd);
	} else {
		// Open FIFO for reading
		get_fifo_name(fifo_name, this->swk_id, this->id);
		sw_pfd.fd = open(fifo_name.c_str(), O_RDONLY | O_NONBLOCK);
		this->port_pfds.push_back(sw_pfd);

		// Keep attempting to open write FIFO until Switch K opens it in read mode
		while (1) {
			get_fifo_name(fifo_name, this->id, this->swk_id);
			this->swk_wr_fifo = open(fifo_name.c_str(), O_WRONLY  | O_NONBLOCK);
			if (this->swk_wr_fifo != -1) { break; }
		}
	}
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

	std::cout << "\n";
	// Print flow table entries
	std::cout << SW_PRINT_FT_TABLE_TITLE;
	for (int i = 0; i < ft_len; i++) {
		fprintf(stdout, SW_PRINT_FT_TABLE_IDX, i);
		this->flow_table[i]->print();
	}
	std::cout << "\n";

	// Print packet stats
	this->stats->print();
	std::cout << "\n";
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
	char line_c_str[MAX_LINE_READ];

	memset(line_c_str, 0, MAX_LINE_READ);

	// Check if end of traffic file has been reached.
	if (!(fgets(line_c_str, sizeof(line_c_str), this->tfile))) { return; }
	line = line_c_str;

	// If next line is empty or is a comment (starts with '#'), do nothing.
        if (line.front() == '#' || line.front() == '\n') { return; }
	// Remove newline char from read line
	line = line.substr(0, line.length() - 1);	

	try {
		// Get header from line
		header.deserialize(line);
		if (header.swi == this->id) {
			// Check if header is delay type
			if (header.timeout > 0) {
				this->start_traffic_delay(header.timeout);
			} else {
				// Log admitted header
				this->stats->log_rcv(PT_ADMIT);
				this->handle_header(header);
			}
		} 
	} catch (Header_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_READ_TFILE_FUNC, e.get_traceback(), e.get_error_code()); }
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
 * Throws: 
 *	- Sw_Exception
 */
void Switch::handle_header(Header &header) {
	int ft_len = this->flow_table.size();
	int match_idx = -1;

	try {
		for (int i = 0; i < ft_len; i++) {
			if (this->flow_table[i]->is_match(header)) {
				match_idx = i;
				break;
			}
		}
		if (match_idx < 0) { match_idx = this->query_cont(header); }
		this->execute_rule(header, match_idx);
	} catch (Sw_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_HANDLE_HEADER_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: query_cont
 * -----------------------
 * Send Controller a QUERY Packet containing the serialized Header "header"
 * and wait for an ADD Packet to be sent back. The Rule in the ADD Packet
 * is then added to the flow table.
 *
 * Parameters:
 * 	- header: The Header to be sent to the Controller/
 * Return Value: The index of the new Rule in the flow table.
 * Throws: 
 *	- Sw_Exception
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
		// Wait for ADD Packet, then push new Rule to flow table
		while (1) {
			this->poll_ports();
			if (this->client->is_pkt_from_server()) {
				this->rcv_pkt(add_pkt, CONT_PORT);
				new_rule = new Rule(add_pkt.msg);
				this->flow_table.push_back(new_rule);
				return (this->flow_table.size() - 1);
			}
		}
	} catch (Sw_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_QUERY_CONT_FUNC, e.get_traceback(), e.get_error_code()); }
}

/**
 * Function: execute_rule
 * -----------------------
 * This function is called after having matched the Header "header" to the Rule
 * in the flow table at index "rule_idx".
 * If the Action Type is FORWARD, the Header is forwarded to one of the two adjacent
 * Switches. Otherwise, no action is performed.
 *
 * Parameters:
 * 	- header: The Header to be either forwarded or dropped.
 *	- rule_idx: The flow table index of the Rule that matches the "header".
 * Return Value: None
 * Throws: 
 *	- Sw_Exception
 */
void Switch::execute_rule(Header& header, int rule_idx) {
	Rule * rule;
	Packet relay_pkt;
	std::string ser_header;

	try {
		// If action type is FORWARD, send header to specified port
		rule = this->flow_table[rule_idx];
		if (rule->act_type == AT_FORWARD && rule->act_val != IP_PORT) {
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
 * Function: install_rule
 * -----------------------
 * Creates a new Rule given each required parameter, then pushes it to the flow table.
 *
 * Parameters:
 * 	- src_IP: IP range of header sources.
 *	- dest_IP: IP range of header destinations.
 *	- atype: Action type.
 *	- aval: Action value (i.e. port to forward the Header to, if needed)
 *	- pri: Rule priority.
 * Return Value: None
 * Throws: None
 */
void Switch::install_rule(IP_Range src_IP, IP_Range dest_IP, ActType atype, SwPort aval, int pri) {
	Rule * new_rule = new Rule(src_IP, dest_IP, atype, aval, pri);
	this->flow_table.push_back(new_rule);
}

/**
 * Function: start_traffic_delay
 * -----------------------
 * Start delay on reading of traffic file for "delay" milliseconds.
 *
 * Parameters:
 * 	- delay: Number of milliseconds.
 * Return Value: None
 * Throws: None
 */
void Switch::start_traffic_delay(int delay) {
	std::cout << "\n";
	fprintf(stdout, SW_DELAY_START_MSG, delay);
	this->timer->start(delay);
	this->is_delayed = true;
	std::cout << "\n";
}

/**
 * Function: handle_user_cmd
 * -----------------------
 * This function is intended to be called after polling stdin and finding
 * waiting input.
 * Executes either the "list" or "exit" commands, or outputs an error message
 * for invalid commands. Note that for "exit", a flag is simply set so that
 * the "run" function knows to terminate the Switch.
 *
 * Parameters: None
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
 * Function: stop
 * -----------------------
 * Closes client socket, traffic file, and FIFO file descriptors for adjacent switches
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Switch::stop() {
	this->client->close_client();
	fclose(this->tfile);
	// TODO: Close fifo fds and tfile
	if (this->swj_id > 0) {
		close(this->swj_wr_fifo);
		close(this->port_pfds[0].fd);		
	}
	if (this->swk_id > 0) {
		close(this->swk_wr_fifo);
		close(this->port_pfds[0].fd);
	}
}

/**
 * Function: run
 * -----------------------
 * Calls method for opening connection to Controller server and FIFOs to
 * adjacent switches (if any), then enters main run loop of Switch.
 * On termination, all connections and open file descriptors are closed.
 *
 * Parameters: None
 * Return Value: None
 * Throws:
 *	- Sw_Exception
 */
void Switch::run() {
	Packet pkt;
	Header header;
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
				if (this->is_delayed == true) {
					std::cout << "\n" << SW_DELAY_END_MSG << "\n";
					this->is_delayed = false;
				}
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
			
			// Poll adjacent switches and handle headers from RELAY pkts if any
			this->poll_ports();
			if (this->port_pfds[0].revents & POLLIN) {
				this->port_pfds[0].revents = 0;
				this->rcv_pkt(pkt, SWJ_PORT);
				header.deserialize(pkt.msg);
				this->handle_header(header);
			} else if (this->port_pfds[1].revents & POLLIN) {
				this->port_pfds[1].revents = 0;
				this->rcv_pkt(pkt, SWK_PORT);
				header.deserialize(pkt.msg);
				this->handle_header(header);
			}
			
		} 
	} catch (Sw_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_RUN_FUNC, e.get_traceback(), e.get_error_code()); }
	  catch (Header_Exception& e) { throw Sw_Exception(e.what(), ERR_SW_RUN_FUNC, e.get_traceback(), e.get_error_code()); }
	
	this->stop();
	std::cout << "\n" << SW_EXIT_MSG << "\n";
}
