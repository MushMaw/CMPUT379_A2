#include "fifolib.h"

std::string get_fifo_name(int writer, int reader) {
	std::string fifo_name = STR_FIFO_BASE + std::to_string(writer) + std::string ("-") + std::to_string(reader);
	return fifo_name;
}

size_t read_from_fifo(std::ifstream fifo, std::string& msg, int msg_len) {
	char * msg_buffer = new char [msg_len];
	size_t total_read;

	fifo.read(msg_buffer, msg_len);
	total_read = strlen(msg_buffer);
	
	msg.assign(msg_buffer);
	delete[] msg_buffer;
	return total_read;
}

void write_to_fifo(std::ofstream fifo, const std::string& msg, int msg_len) {
	fifo.write(msg.c_str(), msg_len);
}

// Unused create/delete fifo functions
/**
int create_fifo(int writer, int reader) {
	int new_fifo;
	std::string fifo_name = get_fifo_name(reader, writer);

	new_fifo = mkfifo(fifo_name.c_str(), FIFO_PERMS);
	return new_fifo;
}

void delete_fifo(int writer, int reader) {
	std::string fifo_name = get_fifo_name(reader, writer);
	remove((FIFO_DIR + fifo_name).c_str());
}
*/
