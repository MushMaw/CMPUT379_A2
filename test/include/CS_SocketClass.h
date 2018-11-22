#if !defined(CS_SOCKET_CLASS_H)
#define CS_SOCKET_CLASS_H 1

#include <string>
#include <vector>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "PktClass.h"

#define ERR_CS_SKT_SOCKET_CREATE "Error in creating socket\n"
#define ERR_CS_SKT_SOCKET_BIND "Error in binding socket\n"
#define ERR_CS_SKT_SOCKET_CONNECT "Error in connecting socket\n"
#define ERR_CS_SKT_SOCKET_ACCEPT "Error in accepting connection\n"

#define ERR_CONT_SERVER_CONSTR_FUNC std::string("Cont_Server::Cont_Server()")
#define ERR_CONT_SERVER_ACC_CL_FUNC std::string("Cont_Server::accept_clients()")
#define ERR_CONT_SERVER_POLL_CL_FUNC std::string("Cont_Server::poll_clients()")
#define ERR_CONT_SERVER_SEND_PKT_FUNC std::string("Cont_Server::send_pkt()")
#define ERR_CONT_SERVER_RCV_PKT_FUNC std::string("Cont_Server::rcv_pkt()")

#define ERR_SW_CLIENT_CONSTR_FUNC std::string("Sw_Client::Sw_Client()")
#define ERR_SW_CLIENT_RCV_PKT_FUNC std::string("Sw_Client::rcv_pkt()")
#define ERR_SW_CLIENT_SEND_PKT_FUNC std::string("Sw_Client::send_pkt()")
#define ERR_SW_CLIENT_POLL_SERV_FUNC std::string("Sw_Client::poll_server()")

#define ERR_CS_SKT_BINARY_CONVERT_ADDR "Error in converting network address to binary\n" 
#define ERR_CS_SKT_POLL_ERROR "Error during polling\n"


class CS_Skt_Exception : public Traceback_Exception {
	public:
		CS_Skt_Exception(const char* msg, const std::string cur_func, const std::string func_traceback, int error_code) 
		: Traceback_Exception(msg, cur_func, func_traceback, error_code) {}
		CS_Skt_Exception(const char* msg, const std::string cur_func, int error_code)
		: Traceback_Exception(msg, cur_func, error_code) {}
};

class Cont_Server {
	private:
		int serv_sock, port_num, num_clients;
		std::vector<struct pollfd> server_pfd;
		std::vector<struct pollfd> cl_pfds;
		std::vector<int> cl_socks;
	public:
		Cont_Server(int port_num, int num_clients);

		void accept_clients();
		void poll_clients();
		void close_client(int cl_idx);
		void close_all();

		bool client_is_ready(int cl_idx);
		int get_next_ready_cl();

		size_t send_pkt(Packet& pkt, int cl_idx);
		size_t rcv_pkt(Packet& pkt, int cl_idx);
		
};

class Sw_Client {
	private:
		int cl_socket, port_num;
		std::vector<struct pollfd> pfd;
	public:
		Sw_Client(std::string& address, int port_num);

		void poll_server();
		bool is_pkt_from_server();
		size_t send_pkt(Packet& pkt);
		size_t rcv_pkt(Packet& pkt);

		void close_client();
};

#endif
