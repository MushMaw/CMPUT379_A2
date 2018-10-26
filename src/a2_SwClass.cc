/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

// TODO: Print messages as they are received from ports.

#include "a2_SwClass.h"


Switch::Switch(int argc, char *argv[]) {
	struct stat buffer;
	std::string id_str, swj_id_str, swk_id_str, ip_range_str;

	if (argc != 6) {throw Sw_Exception(ERR_SW_CL_FORMAT);}
	if (stat(argv[2], &buffer) == -1) {throw Sw_Exception(ERR_TFILE_NOT_FOUND);}

	tfile_name = argv[2];
	id_str = argv[1]; swj_id_str = argv[3]; swk_id_str = argv[4];
	ip_range_str = argv[5];

	if ((id = get_sw_val(id_str)) == -1) {throw Sw_Exception(ERR_SW_VAL);}
	if ((swj_id = get_sw_val(swj_id_str)) == -1) {throw Sw_Exception(ERR_SW_VAL);}
	if ((swk_id = get_sw_val(swj_id_str)) == -1) {throw Sw_Exception(ERR_SW_VAL);}

	ip_range = get_ip_range(ip_range_str);
	if (ip_range.low == -1 || ip_range.high == -1 || ip_range.low >= ip_range.high) {
		throw Sw_Exception(ERR_IP_RANGE_INVALID);
	}
}
