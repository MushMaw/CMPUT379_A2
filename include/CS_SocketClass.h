#if !defined(CONTSW_SOCKET_CLASS_H)
#define CONTSW_SOCKET_CLASS_H 1

#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define ERR_SOCKET_CREATE "Error in creating socket\n"
#define ERR_SOCKET_BIND "Error in binding socket\n"
#define ERR_SOCKET_CONNECT "Error in connecting socket\n"
#define ERR_BINARY_CONVERT_ADDR "Error in converting network address to binary\n" 

class CS_Skt_Exception: public std::runtime_error {
	public:
		CS_Skt_Exception(const char* message) : std::runtime_error(message){}
}

class ContSw_Socket {
}

class Cont_Server: public ContSw_Socket {
	private:
		int serv_socket, port_num, num_clients;
		std::vector<struct pollfd> cl_pfds;
		std::vector<int> cl_socks;
	public:
		Cont_Server(int port_num, int num_clients);
		void accept_clients();
		
}

class Sw_Client: public ContSw_Socket {
	private:
		int cl_socket, port_num;
		struct pollfd pfd;
	public
		Sw_Client(std::string& address, int port_num);
}

#endif
