// TODO: Print messages as they are received from ports

#include "a2_ContClass.h"

Controller::Controller(int argc, char *argv[]) {
	int _nswitch;
	std::string nswitch_str;

	if (argc != 3) {throw Cont_Exception(ERR_CONT_CL_FORMAT);}
	
	nswitch_str = argv[2];
	_nswitch = str_to_pos_int(nswitch_str);

	if (_nswitch < 1) {throw Cont_Exception(ERR_NSWITCH_NON_POS);}
	if (_nswitch > MAX_NSWITCH) {throw Cont_Exception(ERR_NSWITCH_EXCEED_MAX);}

	nswitch = _nswitch;
}

int Controller::handle_inc_packets() {
	int p_count = 0;
	return p_count;
}
