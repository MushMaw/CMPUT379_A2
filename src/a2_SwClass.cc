/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

// TODO: Print messages as they are received from ports.

#include "a2_SwClass.h"

Switch::Switch(int id, int swj_id, int swk_id, IP_Range ip_range) : id(id),
								    swj_id(swj_id),
								    swk_id(swk_id),
								    ip_range(ip_range) {}

Switch::Switch(std::string& ser_sw) {
	std::vector<std::string> toks;
	int tok_count;

	tok_count = tok_split(ser_sw, SW_DELIM, toks);

	this->id = str_to_pos_int(toks.at(0));

	std::cout << "0:"<<toks.at(0) << "|1:"<<toks.at(1)<<"|2:" << toks.at(2)<<"|3:" << toks.at(3) << "\n";
	if (toks.at(1) == NULL_PORT) { this->swj_id = 0; }
	else { this->swj_id = str_to_pos_int(toks.at(1)); }
	if (toks.at(2) == NULL_PORT) { this->swk_id = 0; }
	else { this->swk_id = str_to_pos_int(toks.at(2)); }

	this->ip_range = get_ip_range(toks.at(3));
}

void Switch::serialize(std::string& ser_sw) {
	std::string ser_ip_range;

	ser_sw += std::to_string(this->id);
	ser_sw += SW_DELIM;

	if (this->swj_id == 0) { ser_sw += NULL_PORT; }
	else { ser_sw += std::to_string(this->swj_id); }
	ser_sw += SW_DELIM;

	if (this->swk_id == 0) { ser_sw += NULL_PORT; }
	else { ser_sw += std::to_string(this->swk_id); }
	ser_sw += SW_DELIM;

	serialize_ip_range(ser_ip_range, this->ip_range);
	ser_sw += ser_ip_range;
}

Switch::Switch(int argc, char *argv[]){
	struct stat buffer;
	std::string id_str, swj_id_str, swk_id_str, ip_range_str;

	if (argc != 6) {throw Sw_Exception(ERR_SW_CL_FORMAT);}
	if (stat(argv[2], &buffer) == -1) {throw Sw_Exception(ERR_TFILE_NOT_FOUND);}

	tfile_name = argv[2];
	id_str = argv[1]; swj_id_str = argv[3]; swk_id_str = argv[4];
	ip_range_str = argv[5];

	if ((id = get_sw_val(id_str)) == -1) {throw Sw_Exception(ERR_SW_VAL);}

	if (swj_id_str == NULL_PORT) { swj_id = 0; }
	else if ((swj_id = get_sw_val(swj_id_str)) == -1) {throw Sw_Exception(ERR_SW_VAL);}
	
	if (swk_id_str == NULL_PORT) { swk_id = 0; }
	else if ((swk_id = get_sw_val(swk_id_str)) == -1) {throw Sw_Exception(ERR_SW_VAL);}

	ip_range = get_ip_range(ip_range_str);
	if (ip_range.low == -1 || ip_range.high == -1 || ip_range.low >= ip_range.high) {
		throw Sw_Exception(ERR_IP_RANGE_INVALID);
	}
}

void Switch::run() {
	std::string ser_sw ("");
	this->serialize(ser_sw);
	
	Packet *pkt = new Packet(PT_OPEN, ser_sw);

	delete pkt;
}
