/**
 * CMPUT 379 - Assignment 3
 * File Name: PktClass.cc
 * Student Name: Jacob Bakker
 *
 */

#include "PktClass.h"

/**
 * Packet Constructors
 */
Packet::Packet(PktType ptype, std::string& msg) : ptype(ptype), msg(msg) {}
Packet::Packet() : ptype(PT_UNINIT), msg("") {};

Packet::Packet(PktType ptype) {
	std::string empty_str("");

	this->ptype = ptype;
	this->msg = empty_str;
}

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
	int type_end_idx, pkt_type, null_idx;

	try {
		type_end_idx = ser_pkt.find(PKT_DELIM);
		if (type_end_idx == 1) {
			pkt_type_str = ser_pkt.substr(0, type_end_idx);
			pkt_type = str_to_int(pkt_type_str);
			if (pkt_type >= 0) {
				this->ptype = static_cast<PktType>(pkt_type);
			}
		} else {
			throw Pkt_Exception(ERR_PKT_SER_FORMAT, ERR_PKT_DESERIALIZE_FUNC, 0);
		}
	} catch (Pkt_Exception& e) { throw Pkt_Exception(e.what(), ERR_PKT_DESERIALIZE_FUNC, e.get_traceback(), e.get_error_code()); }
	  catch (Parse_Exception& e) { throw Pkt_Exception(e.what(), ERR_PKT_DESERIALIZE_FUNC, e.get_traceback(), e.get_error_code()); }
	// Remove padding
	null_idx = ser_pkt.find('\0');
	pkt_msg_str = ser_pkt.substr(type_end_idx + 1, null_idx - 2);
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
			throw Pkt_Exception(ERR_PKT_READ, ERR_PKT_READ_FD_FUNC, 0);
		// If read returns 0, throw error indicating closed fd
		} else if (bytes_read == 0) {
			throw Pkt_Exception(ERR_PKT_READ_CLOSED_WRITE_END, ERR_PKT_READ_FD_FUNC, ERR_CODE_PKT_CLOSED_FD);
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
	} catch (Pkt_Exception& e) { throw Pkt_Exception(e.what(), ERR_PKT_READ_FD_FUNC, e.get_traceback(), e.get_error_code()); }
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
		bytes_wrtn = write(fd, &ser_pkt.c_str()[total_wrtn], (ser_pkt.length() + 1 - total_wrtn));
		if (bytes_wrtn < 0) {
			// Check if read end of fd is closed, otherwise throw generic error
			if (errno == EPIPE) { throw Pkt_Exception(ERR_PKT_WRITE_CLOSED_READ_END, ERR_PKT_WRITE_FD_FUNC, ERR_CODE_PKT_CLOSED_FD); }
			else { throw Pkt_Exception(ERR_PKT_WRITE, ERR_PKT_WRITE_FD_FUNC, 0); }
		} else if (bytes_wrtn == 0) {
			break;
		}
		total_wrtn += bytes_wrtn;
	}
	return total_wrtn;
}
