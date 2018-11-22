#if !defined(CS_LOG_H)
#define CS_LOG_H 1


#include <iostream>

#include "PktClass.h"
#include "constants.h"
#include "parselib.h"

#define PTYPE_STR_OPEN "[OPEN]"
#define PTYPE_STR_ACK "[ACK]"
#define PTYPE_STR_QUERY "[QUERY]"
#define PTYPE_STR_ADD "[ADD]"
#define PTYPE_STR_RELAY "[RELAY]"
#define PTYPE_STR_UNINIT "[UNINIT]"
#define PTYPE_STR_ADMIT "[ADMIT]"

#define LOG_RCV_STR "Received: "
#define LOG_SEND_STR "Transmitted: "
#define LOG_SRC_DEST_STR "(src= %s, dest= %s) "
#define LOG_CONT_STR std::string("cont")

typedef enum {LOG_RCV_MODE, LOG_SEND_MODE} LogMode;

void get_src_dest_str(int sr, std::string& sr_str);
void print_log_start(int src, int dest, LogMode mode, PktType ptype);

#endif
