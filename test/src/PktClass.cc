/**
 * CMPUT 379 - Assignment 3
 * File Name: PktClass.cc
 * Student Name: Jacob Bakker
 *
 */

#include "PktClass.h"

Packet::Packet(PktType ptype, std::string& msg) : ptype(ptype), msg(msg) {}
Packet::Packet() : ptype(PT_UNINIT), msg("") {};

/**
 * Function: set_msg
 * -----------------------
 * Set Packet message to "new_msg".
 *
 * Parameters:
 * 	- new_msg: String to be set for Packet message.
 * Return Value: None
 * Throws: None
 */
void Packet::set_msg(std::string& new_msg) {
	this->msg.assign(new_msg);
}

/**
 * Function: deserialize
 * -----------------------
 * Deserializes Packet into Packet Type and message.
 * Packets should be of form "<type> <msg>" where type is an integer
 * corresponding to the Packet Type while the remaining string is
 * considered the message.
 *
 * Parameters:
 * 	- ser_pkt: Serialized Packet string.
 * Return Value: None
 * Throws: None
 */
void Packet::deserialize(std::string& ser_pkt) {
	std::string pkt_type_str (""), pkt_msg_str ("");
	int type_end_idx, pkt_type;

	try {
		type_end_idx = ser_pkt.find(PKT_DELIM);
		if (type_end_idx == 1) {
			pkt_type_str = ser_pkt.substr(0, type_end_idx);
			pkt_type = str_to_int(pkt_type_str);
			if (pkt_type >= 0) {
				this->ptype = static_cast<PktType>(pkt_type);
			}
		} else {
			throw Packet_Exception(ERR_PKT_SER_FORMAT, ERR_PKT_DESERIALIZE_FUNC);
		}
	} catch (Pkt_Exception& e) { throw; }
	  catch (Parse_Exception& e) { throw Pkt_Exception(e.what(), ERR_PKT_DESERIALIZE_FUNC, e.get_traceback()); }
	pkt_msg_str = ser_pkt.substr(type_end_idx + 1, ser_pkt.length() - 2);
	this->msg = pkt_msg_str;
}

/**
 * Function: serialize
 * -----------------------
 * Serializes Packet attributes into string.
 * Packet length is always fixed, so string is padded with 0s upto
 * PKT_LEN.
 *
 * Parameters:
 * 	- ser_pkt: Stores serialized Packet.
 * Return Value: None
 * Throws: None
 */
void Packet::serialize(std::string& ser_pkt) {
	int ser_pkt_len, pkt_type_int = static_cast<int>(this->ptype);
	std::string pkt_type_str = std::to_string(pkt_type_int);
	

	ser_pkt += (pkt_type_str + PKT_DELIM + this->msg); 
	ser_pkt_len = ser_pkt.length();
	ser_pkt.append(PKT_LEN - ser_pkt_len, '\0'); // Pad remaining pkt space with 0's
}

/**
 * Function: read_from_fd
 * -----------------------
 * Given a file descriptor "fd", reads PKT_LEN bytes from "fd"
 * and deserializes Packet string obtained.
 * This function assumes that the next PKT_LEN bytes in "fd"
 * correspond to a serialized Packet string.
 *
 * Parameters:
 * 	- fd: File descriptor.
 * Return Value: Number of bytes read.
 * Throws:
 *	- Pkt_Exception
 */
size_t Packet::read_from_fd(int fd) {
	std::string ser_pkt(""), buffer_op("");
	char buffer[PKT_LEN + 1];
	size_t total_read = 0, bytes_read = 0;

	memset(buffer, 0, PKT_LEN + 1);
	while (total_read < PKT_LEN + 1) {
		bytes_read = read(fd, buffer, PKT_LEN + 1);
		if (bytes_read < 0) {
			throw Pkt_Exception(ERR_PKT_READ, ERR_PKT_READ_FD_FUNC);
		} else if (bytes_read == 0) {
			break;
		}
		total_read += bytes_read;
		// Save read bytes to string object.
		buffer_op = buffer;
		ser_pkt += buffer_op;
		memset(buffer, 0, PKT_LEN + 1);
	}
	
	// Try to deserialize read string
	try {
		this->deserialize(ser_pkt);
	} catch (Pkt_Exception& e) { throw Pkt_Exception(e.what(), ERR_PKT_READ_FROM_FD_FUNC, e.get_traceback()); }
	return total_read;
}

/**
 * Function: write_to_fd
 * -----------------------
 * Given a file descriptor "fd", writes serialized Packet to fd.
 *
 * Parameters:
 * 	- fd: File descriptor.
 * Return Value: None
 * Throws: None
 */
size_t Packet::write_to_fd(int fd) {
	std::string ser_pkt("");
	size_t total_wrtn = 0, bytes_wrtn = 0;

	this->serialize(ser_pkt);
	while(total_wrtn < PKT_LEN + 1) {
		num_wrtn = write(fd, &ser_pkt.c_str()[total_wrtn], (ser_pkt.length() + 1 - total_wrtn));
		if (bytes_wrtn < 0) {
			throw Pkt_Exception(ERR_PKT_WRITE, ERR_PKT_WRITE_FD_FUNC);
		} else if (bytes_wrtn == 0) {
			break;
		}
	}
	return total_wrtn;
}

/**
 * Function: print_log
 * -----------------------
 * Prints formatted message containing the Packet type, source and destination,
 * whether it was receieved or sent, and the contents of its message.
 * 
 * Note that source/destination are represented as numbers from 0 to MAX_NSWITCH.
 * 0 represents the Controller, while 1 to MAX_NSWITCH represent Switches 
 *
 * Message contents consist of the deserialized object stored in the message (e.g.
 * if ptype=ADD, message is a serialized Rule so deserialize it and call the Rule
 * class' print() method).
 *
 * Parameters:
 * 	- src: Packet sender
 *	- dest: Packet receiver
 *	- mode: Either "receive" or "send" mode
 * Return Value: None
 * Throws:
 *	- Pkt_Exception
 */
void Packet::print_log(int src, int dest, PktLogMode mode) {
	std::string src_str(""), dest_str("");
	Header header;
	Switch swi;
	Rule rule;

	// Print whether Packet was received/sent
	if (mode == PKT_LOG_RCV_MODE) {
		std::cout << PKT_LOG_RCV_STR;
	} else {
		std::cout << PKT_LOG_SEND_STR;
	}

	// Print Packet source and destination
	get_src_dest_str(src, src_str);
	get_src_dest_str(dest, dest_str);
	fprintf(stdout, PKT_LOG_SRC_DEST_STR, src_str.c_str(), dest_str.c_str());
	
	try {
		// Print Packet type and contents of message
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
	} catch (Sw_Exception& e) { throw Pkt_Exception(e.what(), ERR_PKT_PRINT_LOG_FUNC, e.get_traceback()); }
	  catch (Header_Exception& e) { throw Pkt_Exception(e.what(), ERR_PKT_PRINT_LOG_FUNC, e.get_traceback()); }
	  catch (Rule_Exception& e) { throw Pkt_Exception(e.what(), ERR_PKT_PRINT_LOG_FUNC, e.get_traceback()); }
}

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
void Packet::get_src_dest_str(int sr, std::string& sr_str) {
	std::string sw_str("");
	if (sr == 0) {
		sr_str.assign(PKT_LOG_CONT_STR);
	} else {
		get_sw_str(sr, sw_str);
		sr_str.assign(sw_str);
	}
}
