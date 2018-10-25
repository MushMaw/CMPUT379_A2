/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

#include "a2_cont.h"

int run_controller(int argc, char *argv[]) {
	Controller *cont = NULL;
	try {
		cont = new Controller(argc, argv);
	} catch (Cont_Exception& e) {
		std::cout << e.what();
	}

	delete cont;

	return 0;
}
