#if !defined(A2_FIFO_H)
#define A2_FIFO_H 1

#include <fstream>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_FIFO_BASE std::string ("fifo-")
#define FIFO_DIR std::string ("tmp/")
#define FIFO_PERMS S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

std::string get_fifo_name(int writer, int reader);
int create_fifo(int writer, int reader);
void delete_fifo(int writer, int reader);
#endif
