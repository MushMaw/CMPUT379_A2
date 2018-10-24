/**
 *  CMPUT 379 - Assignment 2
 *  Student Name: Jacob Bakker
 */

#include "a2sdn.h"
#include "a2cont.h"
#include "a2swi.h"
#include "a2constants.h"

int main(int argc, char *argv[]) {
	std::string mode_arg;

	if (argc < 2) { std::cout << CMD_LINE_FORMAT; return 0; }
	
	mode_arg = argv[1];
	if (mode_arg == CONT_MODE) {
		if (create_controller(argc, argv) == 1) {std::cout << CMD_LINE_FORMAT;}
	} else if (mode_arg.substr(0,2) == SW_MODE) {
		if (create_switch(argc, argv) == 1) {std::cout << CMD_LINE_FORMAT;}
	} else {
		std::cout << CMD_LINE_FORMAT;
	}
	return 0;
}
