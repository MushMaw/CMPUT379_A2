/**
 * CMPUT 379 - Assignment 3
 * File Name: CS_SktClass.cc
 * Student Name: Jacob Bakker
 *
 * Implements TCP server and client classes to be used by Controllers and Switches respectively.
 *
 * Includes functions for polling the server or clients and sending/receiving Packet objects
 */

#include "CS_SocketClass.h"

/**
 * Function: Cont_Server constructor
 * -----------------------
 * Initializes Controller server.
 *
 * Parameters:
 * 	- port_num: Port number.
 *	- num_clients: Number of expected clients.
 * Return Value: None
 * Throws:
 *	- CS_Skt_Exception
 */
Cont_Server::Cont_Server(int port_num, int num_clients) {
	struct sockaddr_in server;
	struct pollfd server_pfd;

	this->port_num = port_num;
	this->num_clients = num_clients;

	// Initalize array for polling clients.
	for (int i = 0; i < (num_clients + 1); i++) {
		this->cl_pfds.push_back(pollfd());
	}

	// Create master socket
	this->serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serv_sock < 0) {
		throw CS_Skt_Exception(ERR_CS_SKT_SOCKET_CREATE, ERR_CONT_SERVER_CONSTR_FUNC, 0);
	}

	// Prepare for polling incoming client connections
	server_pfd.fd = this->serv_sock;
	server_pfd.events = POLLIN;
	server_pfd.revents = 0;
	this->server_pfd.push_back(server_pfd);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port_num);

	if (bind(this->serv_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		throw CS_Skt_Exception(ERR_CS_SKT_SOCKET_BIND, ERR_CONT_SERVER_CONSTR_FUNC, 0);
	}

	// Accepts connections from specified number of clients
	listen(this->serv_sock, num_clients);
}

/**
 * Function: accept_clients
 * -----------------------
 * Accept connections from "num_clients" clients.
 *
 * Parameters: None
 * Return Value: None 
 * Throws: None
 */
void Cont_Server::accept_clients() {
	int n = 0, new_cl_sock = 0;
	struct pollfd * server_pfd_ptr = &(this->server_pfd[0]);
	struct sockaddr_in from;
	socklen_t fromlen;

	// Poll master socket until "num_clients" clients have connected
	while(n < num_clients) {
		if (poll(server_pfd_ptr, 1, 0) < 0) { throw CS_Skt_Exception(ERR_CS_SKT_POLL_ERROR, ERR_CONT_SERVER_ACC_CL_FUNC, 0); }
		if ((n < num_clients) && (this->server_pfd[0].revents & POLLIN)) {
			fromlen = sizeof(from);
			new_cl_sock = accept(serv_sock, (struct sockaddr *) &from, &fromlen);
			if (new_cl_sock < 0) { throw CS_Skt_Exception(ERR_CS_SKT_SOCKET_ACCEPT, ERR_CONT_SERVER_ACC_CL_FUNC, 0); }
			this->cl_socks.push_back(new_cl_sock);

			this->cl_pfds[n].fd = new_cl_sock;
			this->cl_pfds[n].events = POLLIN;
			this->cl_pfds[n].revents = 0;
			n++;
		}
	}
}

/**
 * Function: poll_clients
 * -----------------------
 * Polls clients.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Cont_Server::poll_clients() {
	struct pollfd * cl_pfds_ptr = &(this->cl_pfds[0]);
	if (poll(cl_pfds_ptr, this->num_clients, 0) < 0) {
		throw CS_Skt_Exception(ERR_CS_SKT_POLL_ERROR, ERR_CONT_SERVER_POLL_CL_FUNC, 0);
	}
}

/**
 * Function: client_is_ready
 * -----------------------
 * Returns true if data has been received from client connection
 * with index "cl_idx" in server.
 *
 * Parameters:
 * 	- cl_idx: Index of client in list of server connections.
 * Return Value:
 * 	- True if client has sent data, false otherwise.
 * Throws: None
 */
bool Cont_Server::client_is_ready(int cl_idx) {
	if (this->cl_pfds[cl_idx].revents & POLLIN) {
		return true;
	} else { return false; }
}

/**
 * Function: get_next_ready_cl
 * -----------------------
 * Returns index of next client connection that has sent data.
 *
 * Parameters: None
 * Return Value:
 * 	- Index of next ready client, -1 if none have sent data.
 * Throws: None
 */
int Cont_Server::get_next_ready_cl() {
	for (int i = 0; i < this->num_clients; i++) {
		if (this->client_is_ready(i)) {
			this->cl_pfds[i].revents = 0;
			return i;
		}
	}
	return -1;
}

/**
 * Function: send_pkt (Cont_Server)
 * -----------------------
 * Sends packet to client given by "cl_idx".
 *
 * Parameters:
 * 	- pkt: Packet to send.
 *	- cl_idx: Index of client in server's list of connections.
 * Return Value:
 * 	- Number of bytes sent to client.
 * Throws:
 *	- CS_Skt_Exception
 */
size_t Cont_Server::send_pkt(Packet& pkt, int cl_idx) {
	int cl_fd = 0;
	size_t bytes_wrtn;

	cl_fd = this->cl_socks[cl_idx];
	try {
		bytes_wrtn = pkt.write_to_fd(cl_fd);
	} catch (Pkt_Exception& e) { throw CS_Skt_Exception(e.what(), ERR_CONT_SERVER_SEND_PKT_FUNC, e.get_traceback(), e.get_error_code()); }
	return bytes_wrtn;
}

/**
 * Function: rcv_pkt (Cont_Server)
 * -----------------------
 * Reads Packet from client given by "cl_idx".
 *
 * Parameters:
 * 	- pkt: Stores Packet object received from client.
 *	- cl_idx: Index of client in server's list of connections.
 * Return Value:
 * 	- Number of bytes read from client.
 * Throws:
 *	- CS_Skt_Exception
 */
size_t Cont_Server::rcv_pkt(Packet& pkt, int cl_idx) {
	int cl_fd = 0;
	size_t bytes_read;

	cl_fd = this->cl_socks[cl_idx];
	try {
		bytes_read = pkt.read_from_fd(cl_fd);
	} catch (Pkt_Exception& e) { throw CS_Skt_Exception(e.what(), ERR_CONT_SERVER_RCV_PKT_FUNC, e.get_traceback(), e.get_error_code()); }
	return bytes_read;
}

/**
 * Function: close_all
 * -----------------------
 * Closes all client sockets of server.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Cont_Server::close_all() {
	for (int i = 0; i < num_clients; i++) {
		this->close_client(i);
	}
}

/**
 * Function: close_client
 * -----------------------
 * Closes specific client socket.
 *
 * Parameters: 
 *	- cl_idx: Index of client socket in server's list of clients
 * Return Value: None
 * Throws: None
 */
void Cont_Server::close_client(int cl_idx) {
	if (this->cl_pfds[cl_idx].fd != -1) {
		close(this->cl_pfds[cl_idx].fd);
		this->cl_pfds[cl_idx].fd = -1;
	}
}

/**
 * Function: Sw_Client Constructor
 * -----------------------
 * Initializes Switch client.
 *
 * Parameters:
 * 	- address: Address of server.
 *	- port_num: Port number.
 * Return Value: None
 * Throws:
 *	- CS_Skt_Exception
 */
Sw_Client::Sw_Client(std::string& address, int port_num) {
	struct sockaddr_in server;
	struct pollfd cont_pfd;
	struct hostent *hp = NULL;

	this->port_num = port_num;
	this->cl_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->cl_socket < 0) {
		throw CS_Skt_Exception(ERR_CS_SKT_SOCKET_CREATE, ERR_SW_CLIENT_CONSTR_FUNC, 0);
	}

	hp = gethostbyname(address.c_str());
	if (hp == (struct hostent *) NULL) { std::cout << "Null hp\n"; exit(1); }

	memset((char *) &server, 0, sizeof(server));
	server.sin_family = hp->h_addrtype;
	memcpy((char *) &server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(port_num);

	/**
	 Attempt conversion for dotted-decimal address
	if (inet_pton(AF_INET, address.c_str(), &server.sin_addr) > 0) {
		valid_addr = true;
		throw CS_Skt_Exception(ERR_CS_SKT_BINARY_CONVERT_ADDR, ERR_SW_CLIENT_CONSTR_FUNC);
	}

	 If not dotted-decimal, attempt conversion for symbolic name
	if ((server.inet_addr(
	*/

	if (connect(this->cl_socket, (struct sockaddr *) &server, sizeof(server)) < 0) {
		throw CS_Skt_Exception(ERR_CS_SKT_SOCKET_CONNECT, ERR_SW_CLIENT_CONSTR_FUNC, 0);
	}

	cont_pfd.fd = this->cl_socket;
	cont_pfd.events = POLLIN;
	cont_pfd.revents = 0;
	this->pfd.push_back(cont_pfd);
}

/**
 * Function: rcv_pkt (Sw_Client)
 * -----------------------
 * Reads Packet from server connection.
 *
 * Parameters:
 * 	- pkt: Stores Packet object received from server.
 * Return Value:
 * 	- Number of bytes read from server.
 * Throws:
 *	- CS_Skt_Exception
 */
size_t Sw_Client::rcv_pkt(Packet& pkt) {
	size_t bytes_read;
	try {
		bytes_read = pkt.read_from_fd(this->cl_socket);
		//std::cout << "in sw_client, read success\n";
	} catch (Pkt_Exception& e) { throw CS_Skt_Exception(e.what(), ERR_SW_CLIENT_RCV_PKT_FUNC, e.get_traceback(), e.get_error_code()); }
	return bytes_read;
}

/**
 * Function: send_pkt (Sw_Client)
 * -----------------------
 * Sends Packet to server.
 *
 * Parameters:
 * 	- pkt: Packet object to be sent to server.
 * Return Value:
 * 	- Number of bytes sent to server.
 * Throws:
 *	- CS_Skt_Exception
 */
size_t Sw_Client::send_pkt(Packet& pkt) {
	size_t bytes_wrtn;
	try {
		bytes_wrtn = pkt.write_to_fd(this->cl_socket);
	} catch (Pkt_Exception& e) { throw CS_Skt_Exception(e.what(), ERR_SW_CLIENT_SEND_PKT_FUNC, e.get_traceback(), e.get_error_code()); }
	return bytes_wrtn;
}

/**
 * Function: poll_server
 * -----------------------
 * Polls server for incoming data.
 *
 * Parameters: None
 * Return Value: None
 * Throws:
 *	- CS_Skt_Exception
 */
void Sw_Client::poll_server() {
	struct pollfd * pfd_ptr = &(this->pfd[0]);
	if (poll(pfd_ptr, 1, 0) < 0) {
		throw CS_Skt_Exception(ERR_CS_SKT_POLL_ERROR, ERR_SW_CLIENT_POLL_SERV_FUNC, 0);
	}
}

/**
 * Function: is_pkt_from_server
 * -----------------------
 * Returns true if the server has sent data.
 *
 * Parameters: None
 * Return Value:
 * 	- True if server has sent data, false otherwise.
 * Throws: None
 */
bool Sw_Client::is_pkt_from_server() {
	if (this->pfd[0].revents & POLLIN) { 
		this->pfd[0].revents = 0;	
		return true;
	} else { return false; }
}

/**
 * Function: close_client
 * -----------------------
 * Closes Switch client's socket to server.
 *
 * Parameters: None
 * Return Value: None
 * Throws: None
 */
void Sw_Client::close_client() {
	close(this->cl_socket);
}
