/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */


#include "a2swi.h"
#include "a2utility.h"
#include "a2constants.h"
class Switch {
	public:

		struct ip_range ip_range;
		int id, swj_id, swk_id;
		std::string tfile_name;
		FILE *tfile;

		Switch(int id, int swj_id, int swk_id, std::string tfile_name, struct ip_range ip_range) {
			id = id;
			swj_id = swj_id;
			swk_id = swk_id;
			tfile_name = tfile_name;
			ip_range = ip_range;
		}
};

int create_switch(int argc, char *argv[], ) {
	class Switch *new_switch;
	struct stat buffer;
	struct ip_range ip_vals;
	int new_sw, swj, swk;
	std::string new_sw_str, swj_str, swk_str, tfile_name, ip_range_str;

	if (argc != 6) { return 1; }
	if (stat(argv[2], &buffer) == -1) { std::cout << ERR_FILE_NOT_FOUND; return 2;}

	new_sw_str = argv[1];
	tfile_name = argv[2];
	swj_str = argv[3];
	swk_str = argv[4];
	ip_range_str = argv[5];
	
	if ((new_sw = get_sw_val(new_sw_str)) == -1) {
		std::cout << ERR_SW_VAL;
		return 2;
	}
	if (swj_str == NULL_SW_STR) { swj = 0; }
	else if ((swj = get_sw_val(swj_str)) == -1) {
		std::cout << ERR_SW_VAL;
		return 2;
	}
	if (swk_str == NULL_SW_STR) { swk = 0; }
	else if ((swk = get_sw_val(swk_str)) == -1) {
		std::cout << ERR_SW_VAL;
		return 2;
	}
	
	ip_vals = get_ip_range(ip_range_str);
	if (ip_vals.ip_low == -1 || ip_vals.ip_high == -1 || ip_vals.ip_low >= ip_vals.ip_high) {
		std::cout << ERR_IP_RANGE_INVALID;
		return 2;
	}

	std::cout << "Starting switch...\n";
	new_switch = new Switch(new_sw, swj, swk, tfile_name, ip_range);
	return 0;
}

int run_switch() {

	return 0;
}
