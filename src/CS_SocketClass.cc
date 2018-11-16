#include "CS_SocketClass.h"

Cont_Server::Cont_Server(int port_num, int num_clients) {
	int serv_sock;
	struct sockaddr_in server;

	this->port_num = port_num;
	this->num_clients = num_clients;
	for (int i = 0; i < (num_clients + 1); i++) {
		this->cl_pfds.push_back(pollfd());
	}

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock < 0) {
		throw CS_Skt_Exception(ERR_SOCKET_CREATE);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port_num);

	if (bind(serv_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		throw CS_Skt_Exception(ERR_SOCKET_BIND);
		exit(1);
	}

	listen(server, num_clients);
	
	return server;
}

void Cont_Server::accept_clients() {
	int n = 1, fromlen = 0, new_cl_sock = 0;
	struct pollfd * cl_pollfds_ptr = &(this->cl_pollfds[0]);
	struct sockaddr_in from;
	while(1) {
		poll(cl_pollfds_ptr, n, 0);
		if ((n < num_clients) && (this->cl_pollfds[0].revents & POLLIN)) {
			fromlen = sizeof(from);
			new_cl_sock = accept(serv_sock, (struct sockaddr *) &from, &fromlen);
			this->cl_socks.push_back(new_cl_sock);

			this->cl_pollfds[n].fd = new_cl_sock;
			this->cl_pollfds[n].events = POLLIN;
			this->cl_pollfds[n].revents = 0;
			n++;
		}
	}
}

int init_client(std::string& address, int port_num) {
	int cl_sock;
	struct sockaddr_in server;

	cl_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (cl_sock < 0) {
		throw CS_Skt_Exception(ERR_SOCKET_CREATE);
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port_num);

	if (inet_pton(AF_INET, address, &server.sin_addr) <= 0) {
		throw CS_Skt_Exception(ERR_BINARY_CONVERT_ADDR);
	}

	if (connect(cl_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		throw CS_Skt_Exception(ERR_SOCKET_CONNECT);
	}

	return cl_sock;
}


