// TODO: Print messages as they are received from ports

#include "ContClass.h"

static volatile bool sig_caught = false;

static void Controller::handle_list_sig() {
	list_sig_caught = true;
}

Controller::Controller(int argc, char *argv[]) {
	int nswitch, portnum;
	std::string nswitch_str;

	if (argc != (CONT_ARG_COUNT + 1)) {throw Cont_Exception(ERR_CONT_CL_FORMAT);}
	
	nswitch_str = argv[2];
	nswitch = str_to_pos_int(nswitch_str);

	if (nswitch < 1) {throw Cont_Exception(ERR_NSWITCH_NON_POS);}
	if (nswitch > MAX_NSWITCH) {throw Cont_Exception(ERR_NSWITCH_EXCEED_MAX);}
	this->nswitch = nswitch;

	portnum_str = argv[3];
	portnum = str_to_pos_int(portnum_str);

	if (portnum < 1) { throw Cont_Exception(ERR_PORTNUM); }
	this->portnum = portnum;
}

int Controller::handle_inc_packets() {
	int p_count = 0;
	return p_count;
}

void Controller::run() {
	
}
