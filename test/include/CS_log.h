#if !defined(CS_LOG_H)
#define CS_LOG_H 1


#include <iostream>

#include "constants.h"
#include "parselib.h"

#define LOG_RCV_STR "Received: "
#define LOG_SEND_STR "Transmitted: "
#define LOG_SRC_DEST_STR "(src= %s, dest= %s) "
#define LOG_CONT_STR std::string("cont")

typedef enum {LOG_RCV_MODE, LOG_SEND_MODE} LogMode;

#endif
