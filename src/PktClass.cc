#include "PktClass.h"

Packet::Packet(PktType ptype, std::string& msg) : ptype(ptype), msg(msg) {}
Packet::Packet() : ptype(PT_UNINIT), msg("") {};

void Packet::set_msg(std::string& new_msg) {
	this->msg.assign(new_msg);
}

void Packet::deserialize(std::string& ser_pkt) {
	std::string pkt_type_str (""), pkt_msg_str ("");
	int type_end_idx, pkt_type;

	type_end_idx = ser_pkt.find(PKT_DELIM);
	if (type_end_idx == 1) {
		pkt_type_str = ser_pkt.substr(0, type_end_idx);
		pkt_type = str_to_pos_int(pkt_type_str);
		if (pkt_type >= 0) {
			this->ptype = static_cast<PktType>(pkt_type);
		}
	}

	pkt_msg_str = ser_pkt.substr(type_end_idx + 1, ser_pkt.length() - 2);
	this->msg = pkt_msg_str;
}

void Packet::serialize(std::string& ser_pkt) {
	int ser_pkt_len, pkt_type_int = static_cast<int>(this->ptype);
	std::string pkt_type_str = std::to_string(pkt_type_int);
	

	ser_pkt += (pkt_type_str + PKT_DELIM + this->msg); 
	ser_pkt_len = ser_pkt.length();
	ser_pkt.append(PKT_LEN - ser_pkt_len, '\0'); // Pad remaining pkt space with 0's
}

/**
size_t Packet::read_from_fifo(int fifo) {
	std::string ser_pkt ("");
	char * msg_buffer = new char [PKT_LEN];
	size_t total_read = 0, num_read = 0;

	while (total_read < PKT_LEN + 1) {
		num_read = read(fifo, msg_buffer, PKT_LEN + 1);
		total_read += num_read;
		std::cout << "read " << total_read << "\n";
	}	

	ser_pkt.assign(msg_buffer);
	delete[] msg_buffer;
	this->deserialize(ser_pkt);
	return total_read;
}

void Packet::write_to_fifo(int fifo) {
	std::string ser_pkt ("");
	size_t total_wrtn = 0, num_wrtn = 0;

	this->serialize(ser_pkt);
	std::cout << "len of ser pkt:" << ser_pkt.length();
	while (total_wrtn < PKT_LEN + 1) {
		std::cout << "Amount to write: " << (ser_pkt.length() + 1 - total_wrtn) << "\n";
		num_wrtn = write(fifo, ser_pkt.c_str(), (ser_pkt.length() + 1 - total_wrtn));
		total_wrtn += num_wrtn;
		std::cout << "wrote " << num_wrtn << "\n";
	}
}
*/

size_t Packet::read_from_fd(int fd) {
	std::string ser_pkt(""), buffer_op("");
	char buffer[PKT_LEN + 1];
	size_t total_read = 0, bytes_read = 0;

	memset(buffer, 0, PKT_LEN + 1);
	while (total_read < PKT_LEN + 1) {
		bytes_read = read(fd, buffer, PKT_LEN + 1);
		if (bytes_read < 0) {
			throw Pkt_Exception(ERR_PKT_READ);
		} else if (bytes_read == 0) {
			break;
		}
		total_read += bytes_read;
		buffer_op = buffer;
		ser_pkt += buffer_op;
		memset(buffer, 0, PKT_LEN + 1);
	}
	
	this->deserialize(ser_pkt);
	return total_read;
}

size_t Packet::write_to_fd(int fd) {
	std::string ser_pkt("");
	size_t total_wrtn = 0, bytes_wrtn = 0;

	this->serialize(ser_pkt);
	while(total_wrtn < PKT_LEN + 1) {
		num_wrtn = write(fd, &ser_pkt.c_str()[total_wrtn], (ser_pkt.length() + 1 - total_wrtn));
		if (bytes_wrtn < 0) {
			throw Pkt_Exception(ERR_PKT_WRITE);
		} else if (bytes_wrtn == 0) {
			break;
		}
	}
	return total_wrtn;
}

void Packet::print_log(int src, int dest, PktLogMode mode) {
	std::string src_str(""), dest_str("");
	Header header;
	Switch swi;
	Rule rule;

	if (mode == PKT_LOG_RCV_MODE) {
		std::cout << PKT_LOG_RCV_TEXT;
	} else {
		std::cout << PKT_LOG_SEND_TEXT;
	}

	get_src_dest_str(src, src_str);
	get_src_dest_str(dest, dest_str);
	fprintf(stdout, PKT_LOG_SRC_DEST_STR, src_str.c_str(), dest_str.c_str());
	
	switch(this->ptype) {
		case PT_OPEN:
			std::cout  << PTYPE_STR_OPEN;
			swi = Switch(this->msg);
			swi.print();
			break;
		case PT_ACK:
			std::cout << PTYPE_STR_ACK << "\n";
			break;
		case PT_QUERY:
			std::cout << PTYPE_STR_QUERY;
			header = Header(this->msg);
			header.print();
			break;
		case PT_ADD:
			std::cout << PTYPE_STR_ADD;
			rule = Rule(this->msg);
			rule.print();
			break;
		case PT_RELAY:
			std::cout << PTYPE_STR_RELAY;
			header = Header(this->msg);
			header.print();
			break;
	}
}

void Packet::get_src_dest_str(
