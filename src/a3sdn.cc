/**
 * CMPUT 379 - Assignment 3
 * File Name: a3sdn.cc
 * Student Name: Jacob Bakker
 *
 */

#include "a3sdn.h"

/**
 * Function: main
 * -----------------------
 * Creates and runs either a Controller or Switch using command-line arguments.
 *
 * If the Controller/Switch terminates due to an error, the error message and a
 * traceback of function calls is displayed.
 *
 * Parameters:
 * 	- argc: Number of arguments.
 *	- argv: Array of arguments.
 *
 * Return Value: 0
 * Throws: None
 */
int main(int argc, char *argv[]) {
	std::string mode_arg;
	Controller *new_cont = NULL;
	Switch *new_sw = NULL;	

	if (argc < 2) { std::cout << ERR_CONT_CL_FORMAT << ERR_SW_CL_FORMAT; return 0; }
	
	mode_arg = argv[1];
	if (mode_arg == CONT_MODE) {
		std::cout << "Running controller...\n";
		try {
			new_cont = new Controller(argc, argv);
			new_cont.run();
		} catch (Cont_Exception& e) { std::cout << e.what(); }
	} else if (mode_arg.substr(0,2) == SW_MODE) {
		std::cout << "Running switch...\n";
		try {
			new_sw = new Switch(argc, argv);
			new_sw->run();
		} catch (Sw_Exception& e) { std::cout << e.what(); }
		delete new_sw;
	} else {
		std::cout << ERR_CONT_CL_FORMAT << ERR_SW_CL_FORMAT;
	}
	return 0;
}
