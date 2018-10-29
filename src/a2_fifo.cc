#include "a2_fifo.h"

std::string get_fifo_name(int writer, int reader) {
	std::string fifo_name = STR_FIFO_BASE + std::to_string(writer) + std::string ("-") + std::to_string(reader);
	return fifo_name;
}

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
