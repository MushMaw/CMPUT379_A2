#include "tcpserver.h"

int init_server(int port_num, int num_clients) {
	int serv_sock;
	struct sockaddr_in server;

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock < 0) {
		//TODO: Err handle
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port_num);

	if (bind(serv_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		//TODO: Err handle
		exit(1);
	}

	listen(server, num_clients);
	
	return server;
}

void accept_n_clients(int serv_sock, int num_clients, std::vector<int>& cl_socks, std::vector<struct pollfd>& cl_pollfds) {
	int n = 1, fromlen = 0, new_cl_sock = 0;
	struct pollfd * cl_pollfds_ptr = &cl_pollfds[0];
	struct sockaddr_in from;
	while(1) {
		poll(cl_pollfds_ptr, n, 0);
		if ((n < num_clients) && (cl_pollfds[0].revents & POLLIN)) {
			fromlen = sizeof(from);
			new_cl_sock = accept(serv_sock, (struct sockaddr *) &from, &fromlen);
			cl_pollfds.pushback(new_cl_sock);

			cl_pollfds[n].fd = new_cl_sock;
			cl_pollfds[n].events = POLLIN;
			cl_pollfds[n].revents = 0;
			n++;
		}
	}
}

int init_client(std::string& address, int port_num) {
	int cl_sock;
	struct sockaddr_in server;

	cl_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (cl_sock < 0) {
		//TODO: e handle
		exit(1);
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port_num);

	if (inet_pton(AF_INET, address, &server.sin_addr) <= 0) {
		//TODO: e handle
		exit(1);
	}

	if (connect(cl_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		//TODO: err handle
		exit(1);
	}

	return cl_sock;
}


