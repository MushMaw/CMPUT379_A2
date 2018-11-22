/**
 * CMPUT 379 - Assignment 3
 * File Name: parselib.cc
 * Student Name: Jacob Bakker
 *
 * Implements basic string parsing functions such as splitting a string into tokens and converting
 * strings to integers.
 * Additionally, contains functions getting names of Switches or FIFOs given Switch ID(s) as well as
 * extracting Switch IDs from those names.
 */


#include "parselib.h"

/**
 * Function: get_sw_val
 * -----------------------
 * Given a string of the format "sw<i>", where i is some integer,
 * return i.
 *
 * Parameters:
 * 	- sw_str: String with above format.
 * Return Value:
 * 	- Switch number.
 * Throws:
 *	- Parse_Exception
 */
int get_sw_val(std::string const& sw_str) {
	int sw_num;
	std::string sw_num_substr;
	try {
		if (sw_str.substr(0,2) == SW_MODE) {
			sw_num_substr = sw_str.substr(2, sw_str.length());
			sw_num = str_to_int(sw_num_substr);
			return sw_num;
		}
	} catch (Parse_Exception& e) { throw Parse_Exception(e.what(), ERR_PARSELIB_GET_SW_VAL_FUNC, e.get_traceback(), e.get_error_code()); }

	// If first two chars of "sw_str" are not "sw", throw exception
	throw Parse_Exception(ERR_PARSELIB_SW_NUM_FORMAT, ERR_PARSELIB_GET_SW_VAL_FUNC, 0);
}

/**
 * Function: get_sw_str
 * -----------------------
 * Sets "sw_str" to a string of the format "sw<sw_val>".
 *
 * Parameters:
 * 	- sw_val: A positive Switch number.
 *	- sw_str: Stores the Switch string with the above format.
 * Return Value: None
 * Throws:
 *	- Parse_Exception
 */
void get_sw_str(int sw_val, std::string& sw_str) {
	if (sw_val <= 0) { throw Parse_Exception(ERR_PARSELIB_NON_POS_SW_VAL, ERR_PARSELIB_GET_SW_STR_FUNC, 0); }
	sw_str.clear();
	sw_str += SW_MODE;
	sw_str += std::to_string(sw_val);
}

/**
 * Function: str_to_int
 * -----------------------
 * Returns integer conversion of string.
 *
 * Parameters:
 * 	- str: String to be converted to integer.
 * Return Value:
 * 	- Integer conversion of "str".
 * Throws:
 *	- Parse_Exception
 */
int str_to_int(std::string const& str) {
	int output;
	char *c_ptr;

	output = strtol(str.c_str(), &c_ptr, 10);
	// Throw exception if non-numeric character is found in "str"
	if (*c_ptr) { throw Parse_Exception(ERR_PARSELIB_NON_INT_CHAR, ERR_PARSELIB_STR_TO_INT_FUNC, 0); }
	
	return output;
}

/**
 * Function: tok_split
 * -----------------------
 * Splits string "str" into tokens by the delimiter "delim".
 *
 * Parameters:
 * 	- str: String to be split into tokens.
 *	- delim: String delimiter to split "str" by.
 *	- toks: Stores vector of string tokens taken from "str".
 * Return Value:
 * 	- Number of tokens taken from "str".
 * Throws: None
 */
int tok_split(std::string& str, std::string delim, std::vector<std::string>& toks) {
	int tok_start = 0, tok_end = -1, count = 0;

	tok_end = str.find(delim);
	while (tok_end >= 0) {
		toks.push_back(str.substr(tok_start, tok_end - tok_start));
		tok_start = tok_end + 1;
		count++;
		tok_end = str.find(delim, tok_start);
	}

	// If characters exist after the last delimiter instance, save them as a token.
	if (tok_end < 0 && (unsigned)tok_start < str.length()) {
		toks.push_back(str.substr(tok_start, str.length() - tok_start));
		count++;
	}
	return count;
}

/**
 * Function: get_fifo_name
 * -----------------------
 * Constructs and returns fifo name using format "fifo-<writer>-<reader>", where 
 * both "writer" and "reader" are integers.
 *
 * Parameters:
 * 	- fifo_name: Stores fifo name.
 *	- writer: Integer id of writer to fifo.
 *	- reader: Integer id of reader to fifo. 
 * Return Value: None
 * Throws: None
 */
void get_fifo_name(std::string& fifo_name, int writer, int reader) {
	fifo_name.clear();
	fifo_name = STR_FIFO_BASE + std::to_string(writer) + std::string ("-") + std::to_string(reader);
}
