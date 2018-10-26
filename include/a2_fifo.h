#if !defined(A2_FIFO_H)
#define A2_FIFO_H 1

#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#define STR_FIFO_BASE std::string ("fifo-")
#define FIFO_DIR std::string ("tmp/")
#define FIFO_PERMS S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

std::string get_fifo_name(int reader, int writer);
int create_fifo(int reader, int writer);
void delete_fifo(int reader, int writer);
#endif
