/**
 * CMPUT 379 - Assignment 3
 * File Name: a3sdn.cc
 * Student Name: Jacob Bakker
 *
 * Implements basic main function for taking command line arguments and determining
 * whether to start a Controller or Switch based on the first argument. The arguments
 * are then supplied to the appropriate class which handles their parsing and execution.
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
	// If 1st argument if "cont", attempt to start Controller
	if (mode_arg == CONT_MODE) {
		std::cout << "Running controller...\n";
		try {
			new_cont = new Controller(argc, argv);
			new_cont->run();
		} catch (Cont_Exception& e) { e.print_traceback(); }
		delete new_cont;

	// If 1st argument is "swi", attempt to start Switch
	} else if (mode_arg.substr(0,2) == SW_MODE) {
		std::cout << "Running switch...\n";
		try {
			new_sw = new Switch(argc, argv);
			new_sw->run();
		} catch (Sw_Exception& e) { e.print_traceback(); }
		delete new_sw;

	// Otherwise, display templates for command line arguments
	} else {
		std::cout << ERR_CONT_CL_FORMAT << ERR_SW_CL_FORMAT;
	}
	return 0;
}
