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

void Cont_Server::poll_clients() {
	struct pollfd * cl_pollfds_ptr = &(this->cl_pollfds[0]);
	poll(cl_pollfds_ptr, this->num_clients, 0);
}

int Cont_Server::get_next_ready_cl() {
	for (int i = 0; i < this->num_clients; i++) {
		if (this->cl_pfds[i].revents & POLLIN) {
			this->cl_pfds[i].revents = 0;
			return i;
		}
	}
	return -1;
}

Sw_Client::Sw_Client(std::string& address, int port_num) {
	int cl_sock;
	struct sockaddr_in server;

	this->port_num = port_num;
	this->cl_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->cl_socket < 0) {
		throw CS_Skt_Exception(ERR_SOCKET_CREATE);
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port_num);

	if (inet_pton(AF_INET, address, &server.sin_addr) <= 0) {
		throw CS_Skt_Exception(ERR_BINARY_CONVERT_ADDR);
	}

	if (connect(this->cl_socket, (struct sockaddr *) &server, sizeof(server)) < 0) {
		throw CS_Skt_Exception(ERR_SOCKET_CONNECT);
	}

	this->pfd.fd = this->cl_socket;
	this->pfd.events = POLLIN;
	this->pfd.revents = 0;
}

size_t CS_Socket::send_pkt(Packet& pkt, int cl_idx) {
	int cl_fd = 0;
	size_t bytes_wrtn;

	cl_fd = this->cl_socks[cl_idx];
	bytes_wrtn = pkt.write_to_fd(cl_fd);
	return bytes_wrtn;
}

size_t CS_Socket::rcv_pkt(Packet& pkt, int cl_idx) {
	int cl_fd = 0;
	size_t bytes_read;

	cl_fd = this->cl_socks[cl_idx];
	bytes_read = pkt.read_from_fd(cl_fd);
	return bytes_read;
}
/**
int CS_Socket::send_pkt(int dest_sock, std::string& pkt, int pkt_len){
	int total_sent = 0, bytes_sent = 0;

	while (total_rcv < pkt_len) {
		bytes_sent = write(dest_sock, &pkt.c_str()[total_sent], pkt_len);
		if (bytes_sent < 0) {
			return -1;
		total_sent += bytes_sent;
	}
	return total_sent;
}

int CS_Socket::rcv_pkt(int send_sock, std::string& pkt, int pkt_len) {
	int total_rcv = 0, bytes_rcv = 0;
	char buffer[pkt_len + 1];
	std::string buffer_op;

	while (total_rcv < pkt_len) {
		bytes_rcv = read(send_sock, buffer, pkt_len);
		if (bytes_rcv < 0) {
			return -1;
		} else if (bytes_rcv == 0) {
			break;
		}
		buffer_op = buffer;
		pkt += buffer_op;
		memset(buffer, 0, pkt_len + 1);
	}
	return total_rcv;
}

*/
