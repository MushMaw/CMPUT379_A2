#include "test.h"

void run_serv();
void run_client();
void test_pktstat();
void test_pkt();
void test_rule();

int main(int argc, char *argv[]) {
	if (argc == 2) { run_serv(); }
	else if (argc == 3) { run_client(); }
	else { test_rule(); }
	return 0;
}

void test_rule() {
	Rule * rule = NULL, * deser_rule = NULL;
	IP_Range src_ip(100,200), dest_ip(300,400);
	SwPort act_val = CONT_PORT;
	ActType act_type = AT_DROP;
	int pri = 4;
	std::string ser_rule("");

	std::cout << "Creating and printing new rule...\n";
	rule = new Rule(src_ip, dest_ip, act_type, act_val, pri);
	rule->print();

	std::cout << "Serialize, then unpack into other rule...\n";
	rule->serialize(ser_rule);
	deser_rule = new Rule(ser_rule);
	deser_rule->print();

	Header header(1, 200, 450);
	if (rule->is_match(header)) { std::cout << "Match!\n"; }
	else { std::cout << "no match\n"; }

	std::cout << "Test add pkt...\n";
	rule->print();
	std::cout << "Add pkt\n";
	rule->add_pkt();
	rule->print();
}

void run_serv() {
	Cont_Server * serv = NULL;
	std::string cl_msg("Hello, client!");
	int portnum = 9586, num_cl = 2, next_cl = -1, rcv_num = 0;
	Timer * tim = new Timer();
	Packet pkt, send_pkt(PT_OPEN, cl_msg);

	try {
		std::cout << "Starting server...\n";
		serv = new Cont_Server(portnum, num_cl);

		std::cout << "Starting polling loop for 10sec...\n";
		while (1) {
			//if (tim->at_target_duration()) { break; }
			serv->poll_clients();
			next_cl = serv->get_next_ready_cl();
			if (next_cl > -1) {
				std::cout << next_cl; 
				std::cout << "Client is ready, getting pkt...\n";
				serv->rcv_pkt(pkt, next_cl);
				std::cout << "Msg is: " << pkt.msg << "\n";
				rcv_num++;
				
				std::cout << "Sending msg to client...\n";
				serv->send_pkt(send_pkt, next_cl);
				if (rcv_num == num_cl) { break; }
			}
		}

	} catch (CS_Skt_Exception& e) { 
		if (e.get_error_code() == ERR_CODE_PKT_CLOSED_FD) { std::cout << "Client seems dead\n"; }
		else { e.print_traceback(); std::cout << "Error code: " << e.get_error_code() << "\n"; }
	}
	tim->start(5000);
	while (tim->at_target_duration() == false) {}
	return;
}

void run_client() {
	Sw_Client * client = NULL;
	int portnum = 9586;
	std::string address("localhost"), pkt_msg("Hello, server!");
	Packet pkt(PT_OPEN, pkt_msg), rcv_pkt;
	Timer * wait_tim = new Timer();
	wait_tim->start(10000);
	try {

		std::cout << "Starting client...\n";
		client = new Sw_Client(address, portnum);

		std::cout << "Sending Pkt to server...\n";
		client->send_pkt(pkt);

		while (1) {
			client->poll_server();
			if (client->is_pkt_from_server()) {
				client->rcv_pkt(rcv_pkt);
				std::cout << "From server: " << rcv_pkt.msg << "\n";
				break;
			}
		}
	} catch (CS_Skt_Exception& e) { e.print_traceback(); }
	while (wait_tim->at_target_duration() == false) {}
	return;
}

void test_pkt() {
	try {
		Packet * pkt = new Packet(PT_OPEN);
		Packet * des_pkt = new Packet();
		std::string msg_test = "Please work", ser_test("");

		std::cout << "Try setting msg...\n";
		pkt->set_msg(msg_test);

		if (pkt->ptype == PT_OPEN) { std::cout << "Pkt type is PT_OPEN\n"; }
		std::cout << "Msg is: " << pkt->msg << "\n";

		pkt->serialize(ser_test);
		std::cout << "ser pkt is: " << ser_test << "\n";

		des_pkt->deserialize(ser_test);
		if (des_pkt->ptype == PT_OPEN) { std::cout << "Des ptype is PT_OPEN\n"; }
		std::cout << "des pkt is: " << des_pkt->msg << "\n";

		std::cout << "Len of orig: " << msg_test.length() << "\n";
		std::cout << "Len of deser: " << des_pkt->msg.length() << "\n";

		if (des_pkt->msg == msg_test) { std::cout << "SAME\n"; }
		else { std::cout << "DIFF\n"; }
	} catch (Pkt_Exception& e) { e.print_traceback(); }
}
	

void test_pktstat() {
	ContStats * co_stats = new ContStats();
	SwStats * sw_stats = new SwStats() ;

	// Log 2 OPEN and 1 QUERY
	std::cout << "Log 2 OPEN, 1 QUERY...\n";
	co_stats->log_rcv(PT_OPEN);
	co_stats->log_rcv(PT_OPEN);
	co_stats->log_rcv(PT_QUERY);

	sw_stats->log_send(PT_OPEN);
	sw_stats->log_send(PT_OPEN);
	sw_stats->log_send(PT_QUERY);

	// Log RELAY, 1 in and 2 out
	sw_stats->log_rcv(PT_RELAY);
	sw_stats->log_send(PT_RELAY);
	sw_stats->log_send(PT_RELAY);

	// Print both
	std::cout << "Print cont stats...\n";
	co_stats->print();

	std::cout << "Print sw stats...\n";
	sw_stats->print();
}
