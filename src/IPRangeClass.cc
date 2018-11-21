/**
 * CMPUT 379 - Assignment 3
 * File Name: IPRangeClass.cc
 * Student Name: Jacob Bakker
 *
 */

#include "IPRangeClass.h"

/**
 * IP_Range constructors
 */
IP_Range::IP_Range() {
	this->low = -1;
	this->high = -1;
}

IP_Range::IP_Range(int low, int high) {
	this.low = low;
	this.high = high;
}

IP_Range::IP_Range(std::string& ser_ip) {
	this->deserialize(ser_ip);
}

/**
 * Function: is_in_range
 * -----------------------
 * Returns true if "ip_val" is within IP range.
 *
 * Parameters:
 * 	- ip_val
 * Return Value:
 * 	- True if "ip_val" is in range, false otherwise.
 * Throws: None
 */
bool IP_Range::is_in_range(int ip_val) {
	if (this->low <= ip_val && ip_val <= this->high) {
		return true;
	} else { return false; }
}

/**
 * Function: serialize
 * -----------------------
 * Serialize IP_Range object as string.
 *
 * Parameters:
 * 	- ser_ip: Stores serialized IP_Range object.
 * Return Value: None
 * Throws: None
 */
void IP_Range::serialize(std::string& ser_ip) {
	ser_ip += std::to_string(this->low);
	ser_ip += std::string (IP_RANGE_DELIM);
	ser_ip += std::to_string(this->high);
}

/**
 * Function: deserialize
 * -----------------------
 * Sets IP_Range attributes to those specified in serialized IP_Range string.
 *
 * Parameters:
 * 	- ser_ip: Serialized IP_Range object.
 * Return Value: None
 * Throws:
 *	- Parse_Exception
 */
void IP_Range::deserialize(std::string& ser_ip) {
	int delim_idx;

	if ((delim_idx = ip_str.find("-")) == -1) { throw IP_Range_Exception(ERR_IP_RANGE_FORMAT); }

	try {
		ip_val_str = ip_str.substr(0, delim_idx);
		this->low = str_to_pos_int(ip_val_str);
		ip_val_str = ip_str.substr((delim_idx + 1), ip_str.length());
		this-> = str_to_pos_int(ip_val_str);
	} catch (ParseException& e) { throw IP_Range_Exception(ERR_IP_RANGE_FORMAT); }
}
