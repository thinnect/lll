/**
 * @author Raido Pahtma, Madis Uusjärv
 * @license MIT
*/
#ifndef __LOGGERS_SIM_H
#define __LOGGERS_SIM_H

#ifndef PRINTF_BUFFER_SIZE
#define PRINTF_BUFFER_SIZE 255
#endif

#ifndef MODULE_NAME_LENGTH
#define MODULE_NAME_LENGTH 13
#endif

#define PGM_P const char*
#define PROGMEM
#define PSTR(s) s
#define strlen_P(s) strlen(s)
#define strrchr_P(s, c) strrchr(s, c)
#define vfprintf_P(stream, format, arg) vfprintf(stream, format, arg)
#define printf_P(format, args...) printf(format, ##args)

#define DBG_CHANNEL_NAME "dbgchannel"

#include "__loggers.h"

#endif // __LOGGERS_SIM_H
