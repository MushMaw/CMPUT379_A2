/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <exception>

#define ERR_NSWITCH_VAL "nswitch must be an integer value between 1 and 7\n"
#define ERR_NSWITCH_RANGE "nswitch value is not in the range of representable integer values\n"

int create_controller(int argc, char *argv[]);
