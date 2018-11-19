/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

// TODO: Print messages as they are received from ports.

#include "SwClass.h"

Switch::Switch(int id, int swj_id, int swk_id, IP_Range ip_range) : id(id),
								    swj_id(swj_id),
								    swk_id(swk_id),
								    ip_range(ip_range) {}

Switch::Switch(std::string& ser_sw) {
	std::vector<std::string> toks;
	int tok_count;

	tok_count = tok_split(ser_sw, SW_DELIM, toks);

	try {
		this->id = str_to_pos_int(toks.at(0));

		std::cout << "0:"<<toks.at(0) << "|1:"<<toks.at(1)<<"|2:" << toks.at(2)<<"|3:" << toks.at(3) << "\n";
		if (toks.at(1) == NULL_PORT) { this->swj_id = 0; }
		else { this->swj_id = str_to_pos_int(toks.at(1)); }
		if (toks.at(2) == NULL_PORT) { this->swk_id = 0; }
		else { this->swk_id = str_to_pos_int(toks.at(2)); }

		this->ip_range = get_ip_range(toks.at(3));
	} catch (ParseException& e) { throw Sw_Exception(e.what()); }
}

void Switch::serialize(std::string& ser_sw) {
	std::string ser_ip_range;

	ser_sw += std::to_string(this->id);
	ser_sw += SW_DELIM;

	if (this->swj_id == 0) { ser_sw += NULL_PORT; }
	else { ser_sw += std::to_string(this->swj_id); }
	ser_sw += SW_DELIM;

	if (this->swk_id == 0) { ser_sw += NULL_PORT; }
	else { ser_sw += std::to_string(this->swk_id); }
	ser_sw += SW_DELIM;

	serialize_ip_range(ser_ip_range, this->ip_range);
	ser_sw += ser_ip_range;
}

Switch::Switch(int argc, char *argv[]){
	struct stat buffer;
	std::string id_str, swj_id_str, swk_id_str, ip_range_str;
	std::string address_str, portnum_str;

	if (argc != 8) {throw Sw_Exception(ERR_SW_CL_FORMAT);}
	if (stat(argv[2], &buffer) == -1) {throw Sw_Exception(ERR_TFILE_NOT_FOUND);}

	tfile_name = argv[2];
	id_str = argv[1]; swj_id_str = argv[3]; swk_id_str = argv[4];
	ip_range_str = argv[5];
	address_str = argv[6];
	portnum_str = argv[7];

	try {	
		if ((id = get_sw_val(id_str)) == -1) {throw Sw_Exception(ERR_SW_VAL);}

		if (swj_id_str == NULL_PORT) { swj_id = 0; }
		else if ((swj_id = get_sw_val(swj_id_str)) == -1) {throw Sw_Exception(ERR_SW_VAL);}
	
		if (swk_id_str == NULL_PORT) { swk_id = 0; }
		else if ((swk_id = get_sw_val(swk_id_str)) == -1) {throw Sw_Exception(ERR_SW_VAL);}

		ip_range = get_ip_range(ip_range_str);
		if (ip_range.low == -1 || ip_range.high == -1 || ip_range.low >= ip_range.high) {
			throw Sw_Exception(ERR_IP_RANGE_INVALID);
		}

		this->client = new Sw_Client(address_str, portnum);

	} catch (CS_Skt_Exception& e) { throw Sw_Exception(e.what()); }
	catch (Parse_Exception& e) { throw Sw_Exception(e.what()); }
}

void Switch::print() {
	std::string ser_IP;

	serialize_IP_range(ser_IP, this->IP_range);
	fprintf(stdout, SW_PRINT_INFO, this->swi, this->swj, this->swk, ser_IP);
}

void Switch::poll_ports() {
	struct pollfd * port_pfds_ptr = &(this->port_pfds[0]);
	poll(port_pfds_ptr, SWPORT_COUNT, 0);
}

void Switch::print() {
	std::string ip_range_str;
	serialize_ip_range(ip_range_str, this->ip_range)
	fprintf(stdout, SW_PRINT_MSG, this->id, this->swj, this->swk, ip_range_str.c_str());
}

void Switch::send_pkt(Packet &pkt, SwPort port) {
	try {
		switch(port) {
			case CONT_PORT:
				this->client->send_pkt(pkt);
				break;
			case SWJ_PORT:
				pkt.send_to_fd(this->port_pfds[SWJ_PORT]);
				break;
			case SWK_PORT:
				pkt.send_to_fd(this->port_pfds[SWK_PORT]);
				break;
			default:
				return;
	} catch (CS_Pkt_Exception& e) { throw Sw_Exception(e.what()); }
	catch (Pkt_Exception& e) { throw Sw_Exception(e.what()); }

	this->stats->log_send(pkt);
	this->print_log(pkt, port, PKT_LOG_RCV_MODE);
}

void Switch::rcv_pkt(Packet &pkt, SwPort port) {
	Header header;

	try {
		switch(port) {
			case CONT_PORT:
				this->client->rcv_pkt(pkt);
				break;
			case SWJ_PORT:
				pkt.read_from_fd(this->port_pfds[SWJ_PORT]);
				break;
			case SWK_PORT:
				pkt.read_from_fd(this->port_pfds[SWK_PORT]);
				break;
			default:
				return;
	} catch (CS_Pkt_Exception& e) { throw Sw_Exception(e.what()); }
	catch (Pkt_Exception& e) { throw Sw_Exception(e.what()); }

	this->stats->log_rcv(pkt);
}

void Switch::start() {
	Packet open_pkt, ack_pkt;
	std::string ser_sw("");

	this->serialize(ser_sw);
	open_pkt.ptype = PT_OPEN;
	open_pkt.set_msg(ser_sw);

	this->send_pkt(open_pkt, CONT_PORT);
	while (1) {
		this->poll_ports;
		if (this->port_pfds[CONT_PORT].revents & POLLIN) {
			this->client->rcv_pkt(ack_pkt)
			if (ack_pkt.ptype == PT_ACK) { break; }
		}	
	}
}

void Switch::run() {
	Packet pkt;
	struct pollfd stdin_pfd[1];

	// Send OPEN packet, then wait for ACK packet
	this->start();

	// Prepare for polling stdin
	stdin_pfd[0].fd = STDIN_FILENO;
	stdin_pfd[0].events = POLLIN;
	stdin_pfd[0].revents = 0;

	while (1) {
		// Handle next line (if any) from traffic file
		this->get_next_traffic_pkt();

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

std::string get_fifo_name(int writer, int reader) {
	std::string fifo_name = STR_FIFO_BASE + std::to_string(writer) + std::string ("-") + std::to_string(reader);
	return fifo_name;
}
