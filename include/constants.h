/**
 * CMPUT 379 - Assignment 2
 * Student Name: Jacob Bakker
 */

#if !defined(_A2_CONSTANTS_H)
#define _A2_CONSTANTS_H 1

#define MAX_NSWITCH 7

#define SW_MODE std::string ("sw")
#define CONT_MODE std::string ("cont")
#define NULL_SW_STR std::string ("null")

#define ERR_CONT_CL_FORMAT "format: ./a2sdn cont nSwitch\n"
#define ERR_SW_CL_FORMAT "format: ./a2sdn swi trafficFile [swj|null] [swk|null] ip_low-ip_high\n"

#endif
