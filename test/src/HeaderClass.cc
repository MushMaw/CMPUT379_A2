/**
 * CMPUT 379 - Assignment 3
 * File Name: HeaderClass.cc
 * Student Name: Jacob Bakker
 *
 */

#include "HeaderClass.h"

/**
 * Header Constructors
 */

Header::Header() {
	this->swi = -1;
	this->src_ip = -1;
	this->dest_ip = -1;
	this->timeout = -1;
}

Header::Header(std::string& ser_header) {
	this->deserialize(ser_header);
}

/**
 * Function: deserialize 
 * -----------------------
 * Sets Header's attributes to those specified in serialized Header string.
 *
 * Parameters:
 * 	- ser_header: Serialized Header object
 * Return Value: None
 */
void Header::deserialize(std::string& ser_header) {
	std::vector<std::string> toks, &toks_ptr = toks;
	int count;

	count = tok_split(ser_header, HEADER_DELIM, toks_ptr);
	if (count != 3) { throw Header_Exception(ERR_HEADER_INVALID_FORMAT, ERR_HEADER_DESERIALIZE_FUNC); }

	try {
		this->swi = get_sw_val(toks.at(0));
		// If Header specifies a delay
		if (toks.at(1) == HEADER_DELAY_TYPE) {
			this->timeout = str_to_int(toks.at(1));
			this->src_ip = -1;
			this->dest_ip = -1;
		// Else, Header specifies a source and destination IP
		} else {
			this->src_ip = str_to_int(toks.at(1));
			this->dest_ip = str_to_int(toks.at(2));
			this->timeout = -1;
		}
	} catch (Parse_Exception& e) { throw Header_Exception(e.what(), ERR_HEADER_DESERIALIZE_FUNC, e.get_traceback()); }
}


/**
 * Function: serialize
 * -----------------------
 * Serializes Header object and assigns result to "ser_header".
 *
 * Parameters:
 * 	- ser_header: Stores serialized Header object.
 * Return Value: None
 */
void Header::serialize(std::string& ser_header) {
	std::string sw_str("");

	ser_header.clear();
	get_sw_str(this->swi, sw_str);
	ser_header += sw_str + HEADER_DELIM;
	ser_header += std::to_string(this->src_ip) + HEADER_DELIM;
	ser_header += std::to_string(this->dest_ip);
}

/**
 * Function: print
 * -----------------------
 * Prints Header attributes to stdout.
 *
 * Parameters: None
 * Return Value: None
 */
void Header::print() {
	fprintf(stdout, HEADER_PRINT_MSG, this->src_ip, this->dest_ip);
}
