#include "HeaderClass.h"

/**
 * Header Constructors
 */
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
	if (count != 3) { throw Header_Exception(ERR_HEADER_TOK_COUNT); }

	try {
		this->sw = get_sw_val(toks.at(0));
		this->src_ip = str_to_pos_int(toks.at(1));
		this->dest_ip = str_to_pos_int(toks.at(2));
	} catch (Parse_Exception& e) { throw Header_Exception(e.what()); }
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
	ser_header.clear();
	ser_header += std::to_string(this-sw) + HEADER_DELIM;
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
