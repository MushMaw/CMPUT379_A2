/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

#include "a2cont.h"
#include "a2utility.h"
#include "a2constants.h"

int create_controller(int argc, char *argv[]) {
	int nswitch;
	std::string nswitch_str;

	if (argc != 3) { return 1; }

	nswitch_str = argv[2];
	nswitch = str_to_pos_int(nswitch_str);

	if (nswitch < 1 || MAX_NSWITCH < nswitch) { 
		std::cout << ERR_NSWITCH_VAL;
		return 2;
	}

	std::cout << "Starting controller...\n";
	return 0;
}
