/**
 *  CMPUT 379 - Assignment 2
 *  Student Name: Jacob Bakker
 */

#include "a2sdn.h"

int main(int argc, char *argv[]) {
	std::string mode_arg;

	if (argc < 2) { std::cout << ERR_CONT_CL_FORMAT << ERR_SW_CL_FORMAT; return 0; }
	
	mode_arg = argv[1];
	if (mode_arg == CONT_MODE) {
		std::cout << "Running controller...\n";
	} else if (mode_arg.substr(0,2) == SW_MODE) {
		std::cout << "Running switch...\n";
	} else {
		std::cout << ERR_CONT_CL_FORMAT << ERR_SW_CL_FORMAT;
	}
	return 0;
}
