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

bool Cont_Server::client_is_ready(int cl_idx) {
	if (this->cl_pfds[i].revents & POLLIN) {
		this->cl_pfds[i].revents = 0;
		return true;
	} else { return false; }
}

int Cont_Server::get_next_ready_cl() {
	for (int i = 0; i < this->num_clients; i++) {
		if (this->client_is_ready(i)) {
			this->cl_pfds[i].revents = 0;
			return i;
		}
	}
	return -1;
}

Sw_Client::Sw_Client(std::string& address, int port_num) {
	int cl_sock;
	struct sockaddr_in server;
	struct pollfd cont_pfd;

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

	cont_pfd.fd = this->cl_socket;
	cont_pfd.events = POLLIN;
	cont_pfd.revents = 0;
	this->pfd.pushback(cont_pfd);
}

size_t Cont_Server::send_pkt(Packet& pkt, int cl_idx) {
	int cl_fd = 0;
	size_t bytes_wrtn;

	cl_fd = this->cl_socks[cl_idx];
	try {
		bytes_wrtn = pkt.write_to_fd(cl_fd);
	} catch (Pkt_Exception& e) { throw CS_Skt_Exception(e.what()); }
	return bytes_wrtn;
}

size_t Cont_Server::rcv_pkt(Packet& pkt, int cl_idx) {
	int cl_fd = 0;
	size_t bytes_read;

	cl_fd = this->cl_socks[cl_idx];
	try {
		bytes_read = pkt.read_from_fd(cl_fd);
	} catch (Pkt_Exception& e) { throw CS_Skt_Exception(e.what()); }
	return bytes_read;

size_t Sw_Client::rcv_pkt(Packet& pkt) {
	size_t bytes_read;
	try {
		bytes_read = pkt.read_from_fd(this->cl_socket);
	} catch (Pkt_Exception& e) { throw CS_Skt_Exception(e.what()); }
	return bytes_read;
}

size_t Sw_Client::send_pkt(Packet& pkt) {
	size_t bytes_wrtn;
	try {
		bytes_wrtn = pkt.write_to_fd(this->cl_socket);
	} catch (Pkt_Exception& e) { throw CS_Skt_Exception(e.what()); }
	return bytes_wrtn;
}

void Sw_Client::poll_cont() {
	struct pollfd * pfd_ptr = &(this->pfd[0]);
	poll(pfd_ptr, 1, 0);
}

bool Sw_Client::is_pkt_from_cont() {
	if (this->pfd[0].revents & POLLIN) { 
		this->pfd[0].revents = 0;	
		return true;
	} else { return false; }
}
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
